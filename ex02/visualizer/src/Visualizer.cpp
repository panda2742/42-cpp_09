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

using namespace std;
using namespace ftxui;

Visu::Task::Task(void)
{
	task_id = CLEAR,
	task = "true";
	priority = 0;
}

Visu::Task::Task(TaskID_t task_id_, const string& task_name, unsigned char priority_val)
{
	*this = Task();
	this->task_id = task_id_;
	this->task = task_name;
	this->priority = priority_val;

	this->tmp_file = ".tmp_" + to_string(rand());
	this->tmp_file_err = this->tmp_file + "_err";
}

Visu::Options::Options(void)
{
	selectedRunMode = 0;
	selectCompilationFlags = 0;
	selectedTreatment = 1;
	amountOfElements = "3000";
}

Visu::Result::Result(const string& container_name_)
{
	this->container_name = container_name_;
	this->valgrind_enabled = false;
	this->flags_enabled = false;
	this->is_sorted_before = false;
	this->is_sorted_after = true;
	this->sequence_size = 0;
	this->init_threads_count = 0;
	this->sort_threads_count = 0;
	this->init_time = 0;
	this->sort_time = 0;
	this->heap_summary = {0, 0, 0};
	this->errors_summary  = {0, 0};
}

ostream&	operator<<(ostream& os, Visu::Result_t res)
{
	os << C_BOLD "Metrics for: " C_BLUE << res.container_name << C_RESET << "\n"
		<< "\tValgrind: " << (res.valgrind_enabled ? C_GREEN "activated" : C_RED "desactivated") << C_RESET << "\n"
		<< "\tOptimization flags: " << (res.flags_enabled ? C_GREEN "activated" : C_RED "desactivated") << C_RESET << "\n"
		<< "\tFlow: " << (res.is_sorted_before ? C_GREEN "sorted" : C_RED "not sorted") << C_RESET << " -> " << (res.is_sorted_after ? C_GREEN "sorted" : C_RED "not sorted") << C_RESET << "\n"
		<< "\tSequence size: " C_PURPLE << res.sequence_size << C_RESET "\n"
		<< "\tThreads used at init: " C_PURPLE << res.init_threads_count << C_RESET "\n"
		<< "\tThreads used at sorting: " C_PURPLE << res.sort_threads_count << C_RESET "\n"
		<< "\tInit time: " C_PURPLE << res.init_time << "µs" C_RESET "\n"
		<< "\tSorting time: " C_PURPLE << res.sort_time << "µs" C_RESET "\n\n";

	return os;
}

Visualizer::Visualizer(void): amount(3000)
{
	options = Visu::Options_t();
}

void	Visualizer::Launch(void)
{
	auto	screen = ScreenInteractive::Fullscreen();

	vector<string>	flagsEntries = {
		"Compile without optimization",
		"Compile with optimizations flags",
		"Test both"
	};
	auto	flagsRadiobox = Wrap_("1. Compilation flags", Radiobox(&flagsEntries, &options.selectCompilationFlags));

	vector<string>	runModeEntries = {
		"Run normally",
		"Run with Valgrind",
		"Test both"
	};
	auto	runModeRadiobox = Wrap_("2. Run mode", Radiobox(&runModeEntries, &options.selectedRunMode));

	auto	input_validator = [&]
	{
		options.amountOfElements.erase(
			remove_if(
				options.amountOfElements.begin(),
				options.amountOfElements.end(),
				[](char c)
				{
					return !isdigit(c);
				}
			),
			options.amountOfElements.end()
		);

		if (!options.amountOfElements.empty())
		{
			try
			{
				amount = stoul(options.amountOfElements);
				if (amount > MAX_VALUE)
				{
					amount = MAX_VALUE;
					options.amountOfElements = MAX_VALUE_STR;
				}
			}
			catch(const exception& e)
			{
				amount = 0;
				options.amountOfElements = "0";
			}
		}
		else
		{
			amount = 0;
			options.amountOfElements = "0";
		}
	};
	auto	input = Wrap_(
		"3. Number of elements",
		Input(&options.amountOfElements, "Size") | CatchEvent([&](Event event)
			{
				if (event == Event::Return)
				{
					input_validator();
					return true;
				}
				return false;
			})
		);

	vector<string>	saveOutputEntries = {
		"Format and save result",
		"Put on standard output",
	};
	auto	treatmentRadiobox = Wrap_("4. Treatment", Radiobox(&saveOutputEntries, &options.selectedTreatment));

	auto	action = [&]
	{
		screen.Exit();
	};
	auto	launchButton = Button("LAUNCH", action, ButtonOption::Animated(Color::Purple));

	auto	layout = Container::Vertical({
		flagsRadiobox,
		runModeRadiobox,
		input,
		treatmentRadiobox,
		launchButton
	});

	auto	layoutComponent = Renderer(layout, [&] {
		return vbox({
			text(" Program launcher & visualizer | Made with <3 by ehosta") | color(Color::BlueViolet) | bold |  border,
			vbox({
				flagsRadiobox->Render(),
				separator(),
				runModeRadiobox->Render(),
				separator(),
				input->Render(),
				separator(),
				treatmentRadiobox->Render(),
				separator(),
				launchButton->Render() | border,
			}) | xflex | size(WIDTH, GREATER_THAN, 40) | border,
		});
	});

	screen.Loop(layoutComponent);
	cout << "\033[2J\033[1;1H" << flush;
	RunProgram_();
}

Component	Visualizer::Wrap_(const string& name, Component component)
{
	return Renderer(component, [name, component] {
		return hbox({
			text(name) | size(WIDTH, EQUAL, 25) | color(Color::SeaGreen1),
			separator(),
			component->Render() | xflex,
		}) | xflex;
	});
}

void	Visualizer::RunProgram_(void)
{
	const string	wait_message = "Running...";
	jthread	spinner([wait_message](stop_token stoken)
		{
			Utils::LoadingSpinner(stoken, wait_message);
		});

	PrepareCommands_();

	for (auto& task : tasks)
		sorted_tasks.push_back(&task.second);

	sort(sorted_tasks.begin(), sorted_tasks.end(),
		[](const Visu::Task *a, const Visu::Task *b)
		{
			return a->priority < b->priority;
		});

	size_t	i = 1;
	for (auto *ptr : sorted_tasks)
	{
		if (!ptr->tmp_file.empty())
		{
			ptr->task += " 1> ";
			if (ptr->tmp_file.starts_with(".tmp"))
				ptr->task += ".visu_tmp/" + ptr->tmp_file;
			else
				ptr->task += ptr->tmp_file;
			ptr->task += " 2> ";
			if (ptr->tmp_file.starts_with(".tmp"))
				ptr->task += ".visu_tmp/" + ptr->tmp_file + "_err";
			else
				ptr->task += ptr->tmp_file + "_err";
		}
		cout << C_CLEARLN C_MINT "[" C_PURPLE << i << C_MINT "/" C_PURPLE << sorted_tasks.size() << C_MINT "] Executing " C_PINK C_BOLD << ptr->task << C_RESET << endl;

		try
		{
			ExecuteCommand_(ptr);
		}
		catch(const std::exception& e)
		{
			CleanTraces_();
			cerr << C_CLEARLN C_RED << ptr->task << ": " << e.what() << C_RESET << endl;
			return;
		}
		
		i++;
	}

	spinner.request_stop();
	CreateMetrics_();
}

void	Visualizer::ExecuteCommand_(Visu::Task *ptr)
{
	int		pclose_retval = 0;
	auto	pclose_exit_value = [&](FILE *file_ptr)
	{
		pclose_retval = pclose(file_ptr);
	};
	unique_ptr<FILE, decltype(pclose_exit_value)>	pipe(popen(ptr->task.c_str(), "r"), pclose_exit_value);

	if (!pipe)
		throw runtime_error("Pipe creation error.");

	array<char, 128>	buffer;
	string				cmd_res;
	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
		cmd_res += buffer.data();

	if (pclose_retval != 0)
		throw runtime_error("Return code for task is error-based.");

	if (ptr->task_id == Visu::TMP_DIR)
	{
		if (filesystem::exists("../../.visu_tmp"))
		{
			if (filesystem::is_directory("../../.visu_tmp"))
				return;
		}
		throw runtime_error("Temporary files folder has not been created.");
	}

	if (ptr->tmp_file_err.empty() || ptr->task.find("valgrind") != string::npos)
		return;

	ifstream	err_file("../../.visu_tmp/" + ptr->tmp_file_err, ios::binary | ios::ate);
	if (!err_file.is_open())
		throw runtime_error("Something went wrong with error file.");
	if (err_file.tellg() != 0)
		throw runtime_error("The error file is actually filled with errors.");
}

void	Visualizer::PrepareCommands_(void)
{
	system("clear");
	// Clean and prepare
	tasks[Visu::TMP_DIR] = Visu::Task(Visu::TMP_DIR, "cd ../../ && mkdir -p .visu_tmp", 0);
	tasks[Visu::TMP_DIR].tmp_file.clear();
	tasks[Visu::TMP_DIR].tmp_file_err.clear();
	tasks[Visu::MAKE_FCLEAN] = Visu::Task(Visu::MAKE_FCLEAN, "cd ../../ && make fclean", 1);

	// Compilation
	if (options.selectCompilationFlags != 1)
		tasks[Visu::MAKE] = Visu::Task(Visu::MAKE, "cd ../../ && make", 2);
	if (options.selectCompilationFlags > 0)
		tasks[Visu::MAKE_BONUS] = Visu::Task(Visu::MAKE_BONUS, "cd ../../ && make bonus", 2);
	
	// Input generation
	tasks[Visu::GENERATE_INPUT] = Visu::Task(
		Visu::GENERATE_INPUT,
		"cd ../../ && shuf -i 1-"
		+ to_string(stoul(options.amountOfElements) * 10) + " -n "
		+ options.amountOfElements + " | tr '\\n' ' '", 3
	);

	// Run programs
	if (options.selectedRunMode != 1)
	{
		if (tasks.find(Visu::MAKE) != tasks.end())
			tasks[Visu::RUN] = Visu::Task(Visu::RUN, "cd ../../ && ./PmergeMe file:.visu_tmp/" + tasks[Visu::GENERATE_INPUT].tmp_file, 4);
		if (tasks.find(Visu::MAKE_BONUS) != tasks.end())
			tasks[Visu::RUN_BONUS] = Visu::Task(Visu::RUN_BONUS, "cd ../../ && ./PmergeMe_with_turbo file:.visu_tmp/" + tasks[Visu::GENERATE_INPUT].tmp_file, 4);
	}
	if (options.selectedRunMode > 0)
	{
		if (tasks.find(Visu::MAKE) != tasks.end())
			tasks[Visu::VALGRIND_RUN] = Visu::Task(Visu::VALGRIND_RUN, "cd ../../ && valgrind " VG_FLAGS " ./PmergeMe file:.visu_tmp/" + tasks[Visu::GENERATE_INPUT].tmp_file, 4);
		if (tasks.find(Visu::MAKE_BONUS) != tasks.end())
			tasks[Visu::VALGRIND_RUN_BONUS] = Visu::Task(Visu::VALGRIND_RUN_BONUS, "cd ../../ && valgrind " VG_FLAGS " ./PmergeMe_with_turbo file:.visu_tmp/" + tasks[Visu::GENERATE_INPUT].tmp_file, 4);
	}
}

void	Visualizer::CreateMetrics_(void)
{
	const string	wait_message = "Analyzing...";
	jthread	spinner([wait_message](stop_token stoken)
		{
			Utils::LoadingSpinner(stoken, wait_message);
		});

	for (auto* ptr : sorted_tasks)
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

void	Visualizer::CleanTraces_(void) const
{
	system("rm -rf ../../.visu_tmp");
	system("cd ../../ && make fclean > /dev/null");
	system("clear");
}

void	Visualizer::DisplayMetrics_(void) const
{
	auto	screen = ScreenInteractive::Fullscreen();

	int	container_selected = 0;
	vector<string> container_entries;
	transform(results.begin(), results.end(), back_inserter(container_entries),
		[](const auto& pair) { return pair.first; }
	);

	auto	menu_component = Menu(&container_entries, &container_selected);
	auto	menu_renderer = Renderer(menu_component, [&]
	{
		return window(text(" Containers ") | color(Color::BlueViolet) | bold, menu_component->Render()  | color(Color::SeaGreen1));
	});
	
	auto	metrics_renderer = Renderer([&]
	{
		if (!container_entries.empty())
		{
			string	metrics = container_entries[container_selected];
			return window(text(" Metrics for " + metrics + " ") | color(Color::BlueViolet) | bold, text(metrics));
		}
		return window(text(" No container selected "), text("No data"));
	});

	auto	layout = Container::Horizontal({
		menu_renderer,
		metrics_renderer
	});
	
	auto	main_renderer = Renderer(layout, [&]
	{
		return hbox({
			menu_renderer->Render() | flex_shrink,
			metrics_renderer->Render() | flex
		});
	});

	screen.Loop(main_renderer);
}
