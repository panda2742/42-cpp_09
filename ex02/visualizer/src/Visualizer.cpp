#include "Visualizer.hpp"
#include "VisualizerOptions.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <filesystem>
#include <ranges>
#include <unistd.h>

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"

void	Visualizer::Launch(void)
{
	visual::ExecutionResultManager	execution_result_manager;
	visual::InterfaceManager		interface_manager;
	visual::TaskManager				task_manager;

	auto	layout = interface_manager.InitLayout(options_);

	std::cout << "\033[2J\033[1;1H" << std::flush;

	const std::string	wait_message = "Running...";
	std::jthread		spinner([wait_message](std::stop_token stoken)
		{
			utils::LoadingSpinner(stoken, wait_message);
		});

	task_manager.PrepareTasks(options_);
	task_manager.RunTasks();
	spinner.request_stop();
	execution_result_manager.GenerateResults(task_manager);

	layout = interface_manager.MetricsLayout(execution_result_manager);
}
