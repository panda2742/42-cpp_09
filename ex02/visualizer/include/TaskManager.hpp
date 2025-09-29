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
		TaskManager(const Visualizer&);
		~TaskManager(void) = default;
		TaskManager(const TaskManager&) = default;
		TaskManager(TaskManager&&) = delete;
		TaskManager&	operator=(const TaskManager&) = default;
		TaskManager&	operator=(TaskManager&&) = delete;

	private:
		const Visualizer&										invoker_;
		std::map<TaskID, Task>									tasks_;
		std::vector<decltype(TaskManager::tasks_)::iterator>	order_;
	};
}
