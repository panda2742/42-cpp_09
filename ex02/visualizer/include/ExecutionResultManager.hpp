#pragma once

#include "ExecutionResult.hpp" // ExecutionResult
#include <map> // map
#include <string> // string
#include <vector> // vector
#include "Visualizer.hpp" // Visualizer

namespace visual
{
	class ExecutionResultManager
	{
	public:
		ExecutionResultManager(Visualizer&);
		~ExecutionResultManager(void) = default;
		ExecutionResultManager(const ExecutionResultManager&) = default;
		ExecutionResultManager(ExecutionResultManager&&) = default;
		ExecutionResultManager&	operator=(const ExecutionResultManager&) = default;
		ExecutionResultManager&	operator=(ExecutionResultManager&&) = default;

		std::map<std::string, std::vector<ExecutionResult>>&	GetResults(void);
		void													GenerateResults(TaskManager&);
	
	private:
		std::map<std::string, std::vector<ExecutionResult>>	results_;
		Visualizer&											invoker_;
	};
}