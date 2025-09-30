#pragma once

#include <map> // map
#include <string> // string
#include <vector> // vector
#include "Visualizer.hpp"
#include "VisualizerOptions.hpp"
#include "Task.hpp"

namespace visual
{
	class TaskManager
	{
	public:
		TaskManager(void) = default;
		~TaskManager(void) = default;
		TaskManager(const TaskManager&) = default;
		TaskManager(TaskManager&&) = delete;

		void					PrepareTasks(VisualizerOptions&);
		void					RunTasks(void);
		std::vector<Task *>&	GetSortedTasks(void);

	private:
		std::map<TaskID, Task>	tasks_;
		std::vector<Task *>		sorted_;
	};
}
