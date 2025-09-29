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
		ExecutionResultManager(const Visualizer);
		~ExecutionResultManager(void) = default;
		ExecutionResultManager(const ExecutionResultManager&) = default;
		ExecutionResultManager(ExecutionResultManager&&) = delete;
		ExecutionResultManager&	operator=(const ExecutionResultManager&) = default;
		ExecutionResultManager&	operator=(ExecutionResultManager&&) = delete;
	
	private:
		std::map<std::string, std::vector<ExecutionResult>>	results;
	};
}