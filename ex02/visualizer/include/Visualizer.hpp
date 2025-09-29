#pragma once

#include "ExecutionResultManager.hpp"
#include "InterfaceManager.hpp"
#include "TaskManager.hpp"
#include "Task.hpp"
#include "VisualizerOptions.hpp"
#include <map>
#include <string>
#include <ostream>

#include "ftxui/component/screen_interactive.hpp"

class Visualizer
{
	public:
		Visualizer(void);
		~Visualizer(void) = default;
		Visualizer(const Visualizer&) = default;
		Visualizer(Visualizer&&) = default;
		Visualizer& operator=(const Visualizer&) = default;
		Visualizer& operator=(Visualizer&&) = default;

		void							Launch(void);
		visual::ExecutionResultManager&	GetExecutionResult(void);
		visual::InterfaceManager&		GetInterfaceManager(void);
		visual::TaskManager&			GetTaskManager(void);
		visual::VisualizerOptions&		GetOptions(void);

	private:
		visual::ExecutionResultManager	execution_result_manager_;
		visual::InterfaceManager		interface_manager_;
		visual::TaskManager				task_manager_;
		visual::VisualizerOptions		options_;

		/**
		 * Analyzes each output of each command and generates some metrics about it.
		 */
		void		CreateMetrics_(void);
		/**
		 * Treat a temporary file and reads its content. Can throw an error if something wrong occurs.
		 */
		void		TreatTask_(visual::Task *ptr);
};
