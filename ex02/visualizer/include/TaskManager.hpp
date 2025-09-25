#pragma once

#include "Task.hpp" // TaskID, Task
#include "Visualizer.hpp" // Visualizer
#include <map> // map
#include <string> // string

namespace visual
{
	class TaskManager
	{
	public:
		TaskManager(const Visualizer&);
		~TaskManager(void) = default;
		TaskManager(const TaskManager&) = default;
		TaskManager(TaskManager&&) = default;
		TaskManager&	operator=(const TaskManager&) = default;
		TaskManager&	operator=(TaskManager&&) = default;

	private:
		const Visualizer&		invoker_;
		std::map<TaskID, Task>	tasks_;
	};
}
