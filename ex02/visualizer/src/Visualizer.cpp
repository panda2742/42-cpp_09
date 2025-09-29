#include "Visualizer.hpp"
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

Visualizer::Visualizer(void)
	: execution_result_manager_(visual::ExecutionResultManager(*this)),
	interface_manager_(visual::InterfaceManager(*this)),
	task_manager_(visual::TaskManager(*this)),
	options_(visual::VisualizerOptions())
	{}

void	Visualizer::Launch(void)
{
	auto	screen = ftxui::ScreenInteractive::Fullscreen();
	screen.Loop(interface_manager_.InitLayout(screen));

	std::cout << "\033[2J\033[1;1H" << std::flush;

	const std::string	wait_message = "Running...";
	std::jthread		spinner([wait_message](std::stop_token stoken)
		{
			utils::LoadingSpinner(stoken, wait_message);
		});

	task_manager_.PrepareTasks();
	task_manager_.RunTasks();
	execution_result_manager_.GenerateResults(task_manager_);

	screen.Loop(interface_manager_.MetricsLayout(screen));

	spinner.request_stop();
	CreateMetrics_();
}

visual::ExecutionResultManager&	Visualizer::GetExecutionResult(void) { return execution_result_manager_; }
visual::InterfaceManager&		Visualizer::GetInterfaceManager(void) { return interface_manager_; }
visual::TaskManager&			Visualizer::GetTaskManager(void) { return task_manager_; }
visual::VisualizerOptions&		Visualizer::GetOptions(void) { return options_; }

void	Visualizer::TreatTask_(Visu::Task *ptr)
{
	ifstream		task_tmp("../../.visu_tmp/" + ptr->tmp_file);

	if (!task_tmp.is_open())
		throw runtime_error(ptr->tmp_file + " is not openable.");
	
	string						line;
	optional<Visu::Result_t>	res;
	while (getline(task_tmp, line))
	{
		vector<string>	parts = Utils::Split(line, "|");

		if (parts.size() == 2 && parts.at(0) == "END")
		{
			string	container_name = Utils::GetLineProperty("container", parts.at(1));

			if (res.has_value() && container_name == res.value().container_name)
			{
				results[res.value().container_name].push_back(res.value());
				res.reset();
			}
			continue;
		}
		if (parts.size() == 3 && parts.at(0) == "START")
		{
			const string	container_name = Utils::GetLineProperty("container", parts.at(1));
			const string	seq_size = Utils::GetLineProperty("size", parts.at(2));
			res = Visu::Result_t(container_name);
			res.value().sequence_size = stoul(seq_size);
			res.value().flags_enabled = ptr->task_id == Visu::RUN_BONUS || ptr->task_id == Visu::VALGRIND_RUN_BONUS;
			res.value().valgrind_enabled = ptr->task_id == Visu::VALGRIND_RUN || ptr->task_id == Visu::VALGRIND_RUN_BONUS;
			continue;
		}

		string	container_name = Utils::GetLineProperty("container", parts.at(0));
		if (!res.has_value() || parts.size() < 2 || container_name != res.value().container_name)
			continue;
		
		vector<string>	keys = Utils::GetLineKeys(line);
		for (const string& key : keys)
		{
			if (key == "threads_init")
			{
				string	value = Utils::GetLineProperty("threads_init", parts.at(1));
				res.value().init_threads_count = stoul(value);
			}
			else if (key == "seq_state")
			{
				string	value = Utils::GetLineProperty("seq_state", parts.at(1));
				if (res.value().init_time == 0)
					res.value().is_sorted_before = value == "sorted";
				else
					res.value().is_sorted_after= value == "sorted";
			}
			else if (key == "time_init")
			{
				string	value = Utils::GetLineProperty("time_init", parts.at(1));
				res.value().init_time = stoul(value);
			}
			else if (key == "threads_sort")
			{
				string	value = Utils::GetLineProperty("threads_sort", parts.at(1));
				res.value().sort_threads_count = stoul(value);
			}
			else if (key == "time_sort")
			{
				string	value = Utils::GetLineProperty("time_sort", parts.at(1));
				res.value().sort_time = stoul(value);
			}
		}
	}

	if (task_tmp.fail() && !task_tmp.eof())
		throw runtime_error("The file " + ptr->tmp_file + " unexpectedly failed/closed.");
	if (task_tmp.bad())
		throw runtime_error("The file " + ptr->tmp_file + " or the disk is corrupted.");
}
