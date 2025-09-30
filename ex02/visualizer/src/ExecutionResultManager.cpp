#include "ExecutionResultManager.hpp" // ExecutionResultManager
#include "Visualizer.hpp" // Visualizer
#include "Utils.hpp" // Utils
#include <iostream>
#include <fstream>

namespace visual
{

std::map<std::string, std::vector<ExecutionResult>>&	ExecutionResultManager::GetResults(void)
{
	return results_;
}

void	ExecutionResultManager::GenerateResults(TaskManager& task_manager)
{
	const std::string	wait_message = "Analyzing...";
	std::jthread		spinner([wait_message](std::stop_token stoken)
		{
			utils::LoadingSpinner(stoken, wait_message);
		});

	for (auto* ptr : task_manager.GetSortedTasks())
	{
		if (ptr->GetTaskID() < TaskID::Run || ptr->GetTaskID() > TaskID::ValgrindRun)
			continue;

		try
		{
			AnalyzeTask(*ptr);
		}
		catch(const std::exception& e)
		{
			utils::CleanTraces();
			std::cerr << C_CLEARLN C_RED << "Task analyze error: " << e.what() << C_RESET << std::endl;
			return;
		}
	}

	utils::CleanTraces();
	spinner.request_stop();
}

void	ExecutionResultManager::AnalyzeTask(Task& task)
{
	std::ifstream	task_tmp("../../.visu_tmp/" + task.GetTmpFile());

	if (!task_tmp.is_open())
		throw std::runtime_error(task.GetTmpFile() + " is not openable.");
	
	std::string						line;
	std::optional<ExecutionResult>	res;
	while (getline(task_tmp, line))
	{
		std::vector<std::string>	parts = utils::Split(line, "|");

		if (parts.size() == 2 && parts.at(0) == "END")
		{
			std::string	container_name = utils::GetLineProperty("container", parts.at(1));

			if (res.has_value() && container_name == res.value().GetContainerName())
			{
				results_[res.value().GetContainerName()].push_back(res.value());
				res.reset();
			}
			continue;
		}
		if (parts.size() == 3 && parts.at(0) == "START")
		{
			const std::string	container_name = utils::GetLineProperty("container", parts.at(1));
			const std::string	seq_size = utils::GetLineProperty("size", parts.at(2));
			res = ExecutionResult(container_name);
			res.value().SetSequenceSize(stoul(seq_size));
			res.value().SetFlagsEnabled(task.GetTaskID() == TaskID::RunBonus || task.GetTaskID() == TaskID::ValgrindRunBonus);
			res.value().SetValgrindEnabled(task.GetTaskID() == TaskID::ValgrindRun || task.GetTaskID() == TaskID::ValgrindRunBonus);
			continue;
		}

		std::string	container_name = utils::GetLineProperty("container", parts.at(0));
		if (!res.has_value() || parts.size() < 2 || container_name != res.value().GetContainerName())
			continue;
		
		std::vector<std::string>	keys = utils::GetLineKeys(line);
		for (const std::string& key : keys)
		{
			if (key == "threads_init")
			{
				std::string	value = utils::GetLineProperty("threads_init", parts.at(1));
				res.value().SetInitThreadsCount(stoul(value));
			}
			else if (key == "seq_state")
			{
				std::string	value = utils::GetLineProperty("seq_state", parts.at(1));
				if (res.value().GetInitTime() == 0)
					res.value().SetSortedBefore(value == "sorted");
				else
					res.value().SetSortedAfter(value == "sorted");
			}
			else if (key == "time_init")
			{
				std::string	value = utils::GetLineProperty("time_init", parts.at(1));
				res.value().SetInitTime(stoul(value));
			}
			else if (key == "threads_sort")
			{
				std::string	value = utils::GetLineProperty("threads_sort", parts.at(1));
				res.value().SetSortThreadsCount(stoul(value));
			}
			else if (key == "time_sort")
			{
				std::string	value = utils::GetLineProperty("time_sort", parts.at(1));
				res.value().SetSortTime(stoul(value));
			}
		}
	}

	if (task_tmp.fail() && !task_tmp.eof())
		throw std::runtime_error("The file " + task.GetTmpFile() + " unexpectedly failed/closed.");
	if (task_tmp.bad())
		throw std::runtime_error("The file " + task.GetTmpFile() + " or the disk is corrupted.");
}

}
