#pragma once

#include "Task.hpp" // TaskID, Task
#include "Visualizer.hpp" // Visualizer
#include <map> // map
#include <string> // string
#include <vector> // vector

namespace visual
{
	class TaskManager
	{
	public:
		TaskManager(Visualizer&);
		~TaskManager(void) = default;
		TaskManager(const TaskManager&) = default;
		TaskManager(TaskManager&&) = delete;
		TaskManager&	operator=(const TaskManager&) = default;
		TaskManager&	operator=(TaskManager&&) = delete;

		/**
		 * Load each command based on the options to then run everything and catch the errors.
		 */
		void	PrepareTasks(void);
		/**
		 * Run each command and display it one by one.
		 */
		void	RunTasks(void);

	private:
		Visualizer&		invoker_;
		std::map<TaskID, Task>	tasks_;
		std::vector<Task *>		sorted_;
	};
}
