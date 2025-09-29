#include "ExecutionResultManager.hpp" // ExecutionResultManager
#include "Visualizer.hpp" // Visualizer

namespace visual
{

ExecutionResultManager::ExecutionResultManager(Visualizer& invoker): invoker_(invoker) {}

std::map<std::string, std::vector<ExecutionResult>>&	ExecutionResultManager::GetResults(void)
{
	return results_;
}

void	ExecutionResultManager::GenerateResults(TaskManager& task_manager)
{
	const std::string	wait_message = "Analyzing...";
	std::jthread		spinner([wait_message](std::stop_token stoken)
		{
			utils::LoadingSpinner(stoken, wait_message);
		});

	for (auto* ptr : invoker_)
	{
		if (ptr->task_id < Visu::RUN || ptr->task_id > Visu::VALGRIND_RUN_BONUS)
			continue;

		try
		{
			TreatTask_(ptr);
		}
		catch(const std::exception& e)
		{
			CleanTraces_();
			cerr << C_CLEARLN C_RED << "Task analyze error: " << e.what() << C_RESET << endl;
			return;
		}
	}

	for (const auto& res_metrics : results)
	{
		cout << "Container " << res_metrics.first << " has " << res_metrics.second.size() << " metrics." << endl;
		for (const auto& res_container_elt : res_metrics.second)
			cout << res_container_elt;
	}

	CleanTraces_();
	spinner.request_stop();
	DisplayMetrics_();
}

}
