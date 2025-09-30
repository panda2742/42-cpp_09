#include <iostream>
#include <string>
#include "Task.hpp"
#include "TaskManager.hpp"
#include "Utils.hpp"
#include "Visualizer.hpp"

namespace visual
{

void	TaskManager::PrepareTasks(VisualizerOptions& options)
{
	system("clear");
	// Clean and prepare
	tasks_.emplace(TaskID::TmpDir, Task(TaskID::TmpDir, "cd ../../ && mkdir -p .visu_tmp", 0));
	tasks_[TaskID::TmpDir].SetTmpFile("");
	tasks_[TaskID::TmpDir].SetTmpFileErr("");
	tasks_.emplace(TaskID::MakeFclean, Task(TaskID::MakeFclean, "cd ../../ && make fclean", 1));

	// Compilation
	if (options.select_compilation_flags != 1)
		tasks_.emplace(TaskID::Make, Task(TaskID::Make, "cd ../../ && make", 2));
	if (options.select_compilation_flags > 0)
		tasks_.emplace(TaskID::MakeBonus, Task(TaskID::MakeBonus, "cd ../../ && make bonus", 2));
	
	// Input generation
	tasks_.emplace(TaskID::GenerateInput, Task(
		TaskID::GenerateInput,
		"cd ../../ && shuf -i 1-"
		+ std::to_string(std::stoul(options.amount_of_elements) * 10) + " -n "
		+ options.amount_of_elements + " | tr '\\n' ' '", 3
	));
	const std::string&	input_filename = tasks_[TaskID::GenerateInput].GetTmpFile();

	// Run programs
	if (options.selected_run_mode != 1)
	{
		if (tasks_.find(TaskID::Make) != tasks_.end())
			tasks_.emplace(TaskID::Run, Task(TaskID::Run,
				"cd ../../ && ./PmergeMe file:.visu_tmp/" + input_filename, 4
			));
		if (tasks_.find(TaskID::MakeBonus) != tasks_.end())
			tasks_.emplace(TaskID::RunBonus, Task(TaskID::RunBonus,
				"cd ../../ && ./PmergeMe_with_turbo file:.visu_tmp/" + input_filename, 4
			));
	}
	if (options.selected_run_mode > 0)
	{
		if (tasks_.find(TaskID::Make) != tasks_.end())
			tasks_.emplace(TaskID::ValgrindRun, Task(TaskID::ValgrindRun,
				"cd ../../ && valgrind " VG_FLAGS " ./PmergeMe file:.visu_tmp/" + input_filename, 4));
		if (tasks_.find(TaskID::MakeBonus) != tasks_.end())
			tasks_.emplace(TaskID::ValgrindRunBonus, Task(TaskID::ValgrindRunBonus,
				"cd ../../ && valgrind " VG_FLAGS " ./PmergeMe_with_turbo file:.visu_tmp/" + input_filename, 4
			));
	}

	// Sort tasks into priority order into the vector.
	for (auto& task : tasks_)
		sorted_.push_back(&task.second);

	sort(sorted_.begin(), sorted_.end(),
		[](const Task *a, const Task *b)
		{
			return a->GetPriority() < b->GetPriority();
		});
}

void	TaskManager::RunTasks(void)
{
	size_t	i = 1;
	for (auto *ptr : sorted_)
	{
		if (!ptr->GetTmpFile().empty())
		{
			ptr->SetTask(ptr->GetTask() + " 1> ");
			if (ptr->GetTmpFile().starts_with(".tmp"))
				ptr->SetTask(ptr->GetTask() + ".visu_tmp/" + ptr->GetTmpFile());
			else
				ptr->SetTask(ptr->GetTask() + ptr->GetTmpFile());
			ptr->SetTask(ptr->GetTask() + " 2> ");
			if (ptr->GetTmpFile().starts_with(".tmp"))
				ptr->SetTask(ptr->GetTask() + ".visu_tmp/" + ptr->GetTmpFile() + "_err");
			else
				ptr->SetTask(ptr->GetTask() + ptr->GetTmpFile() + "_err");
		}
		std::cout << C_CLEARLN C_ORANGE "[" C_PURPLE << i << C_ORANGE "/" C_PURPLE << sorted_.size()
			<< C_ORANGE "] Executing " C_PINK C_BOLD << ptr->GetTask() << C_RESET << std::endl;

		try
		{
			ptr->Execute();
		}
		catch(const std::exception& e)
		{
			utils::CleanTraces();
			std::cerr << C_CLEARLN C_RED << ptr->GetTask() << ": " << e.what() << C_RESET << std::endl;
			return;
		}
		
		i++;
	}
}

std::vector<Task *>&	TaskManager::GetSortedTasks(void) { return sorted_; }

}
