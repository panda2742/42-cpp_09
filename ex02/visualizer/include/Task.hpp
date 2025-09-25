#pragma once

#include <array> // array
#include <string> // string

namespace visual
{
	enum class TaskID
	{
		TmpDir,
		MakeFclean,
		Make,
		MakeBonus,
		Run,
		RunBonus,
		ValgrindRun,
		ValgrindRunBonus,
		SaveOutput,
		GenerateInput,
		Clear
	};

	class Task
	{
	public:
		Task(TaskID);
		~Task(void) = default;
		Task(const Task&) = default;
		Task(Task&&) = default;
		Task&	operator=(const Task&) = default;
		Task&	operator=(Task&&) = default;

	private:
		unsigned char	priority_;
		std::string		task_;
		std::string		tmp_file_;
		std::string		tmp_file_err_;
		TaskID			task_id_;
	};
}
