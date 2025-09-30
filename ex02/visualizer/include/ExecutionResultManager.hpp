#pragma once

#include "ExecutionResult.hpp"
#include "Visualizer.hpp"
#include "Task.hpp"
#include "TaskManager.hpp"
#include <map> // map
#include <string> // string
#include <vector> // vector

namespace visual
{
	class TaskManager;

	class ExecutionResultManager
	{
	public:
		ExecutionResultManager(void) = default;
		~ExecutionResultManager(void) = default;
		ExecutionResultManager(const ExecutionResultManager&) = default;
		ExecutionResultManager(ExecutionResultManager&&) = default;

		std::map<std::string, std::vector<ExecutionResult>>&	GetResults(void);
		void													GenerateResults(TaskManager&);
		void													AnalyzeTask(Task&);
		void													AnalyzeTaskValgrind(Task&, ExecutionResult& res);

	private:
		std::map<std::string, std::vector<ExecutionResult>>	results_;
	};
}