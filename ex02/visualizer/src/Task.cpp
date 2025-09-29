#include <array>
#include <filesystem>
#include <fstream>
#include <ios>
#include <memory>
#include <string>
#include "Task.hpp"

namespace visual
{

Task::Task(TaskID task_id, std::string task, unsigned char priority):
	priority_(priority),
	task_(task),
	tmp_file_(".tmp_" + std::to_string(rand())),
	tmp_file_err_(tmp_file_ + "_err"),
	task_id_(task_id) {}

unsigned char		Task::GetPriority(void) const { return priority_; }
const std::string&	Task::GetTask(void) const { return task_; }
const std::string&	Task::GetTmpFile(void) const { return tmp_file_; }
const std::string&	Task::GetTmpFileErr(void) const { return tmp_file_err_; }
TaskID				Task::GetTaskID(void) const { return task_id_; }

void	Task::SetPriority(unsigned char priority) { priority_ = priority; }
void	Task::SetTask(const std::string& task) { task_ = task; }
void	Task::SetTmpFile(const std::string& tmp_file) { tmp_file_ = tmp_file; }
void	Task::SetTmpFileErr(const std::string& tmp_file_err) { tmp_file_err_ = tmp_file_err; }
void	Task::SetTaskID(TaskID task_id) { task_id_ = task_id; }

void	Task::Execute(void) const
{
	int		pclose_retval = 0;
	auto	pclose_exit_value = [&](FILE *file_ptr)
	{
		pclose_retval = pclose(file_ptr);
	};
	std::unique_ptr<FILE, decltype(pclose_exit_value)>	pipe(popen(task_.c_str(), "r"), pclose_exit_value);

	if (!pipe)
		throw std::runtime_error("Pipe creation error.");

	std::array<char, 128>	buffer;
	std::string				cmd_res;
	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
		cmd_res += buffer.data();

	if (pclose_retval != 0)
		throw std::runtime_error("Return code for task is error-based.");

	if (task_id_ == TaskID::TmpDir)
	{
		if (std::filesystem::exists("../../.visu_tmp"))
		{
			if (std::filesystem::is_directory("../../.visu_tmp"))
				return;
		}
		throw std::runtime_error("Temporary files folder has not been created.");
	}

	if (tmp_file_err_.empty() || task_.find("valgrind") != std::string::npos)
		return;

	std::ifstream	err_file("../../.visu_tmp/" + tmp_file_err_, std::ios::binary | std::ios::ate);
	if (!err_file.is_open())
		throw std::runtime_error("Something went wrong with error file.");
	if (err_file.tellg() != 0)
		throw std::runtime_error("The error file is actually filled with errors.");
}

}
