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
		GenerateInput,
		Clear
	};

	class Task
	{
	public:
		Task(TaskID, std::string, unsigned char);
		~Task(void) = default;
		Task(const Task&) = default;
		Task(Task&&) = default;
		Task&	operator=(const Task&) = default;
		Task&	operator=(Task&&) = default;

		unsigned char		GetPriority(void) const;
		const std::string&	GetTask(void) const;
		const std::string&	GetTmpFile(void) const;
		const std::string&	GetTmpFileErr(void) const;
		TaskID				GetTaskID(void) const;

		void				SetPriority(unsigned char);
		void				SetTask(const std::string&);
		void				SetTmpFile(const std::string&);
		void				SetTmpFileErr(const std::string&);
		void				SetTaskID(TaskID);

		void	Execute(void) const;

	private:
		unsigned char	priority_;
		std::string		task_;
		std::string		tmp_file_;
		std::string		tmp_file_err_;
		TaskID			task_id_;
	};
}
