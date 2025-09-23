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

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"

using namespace std;
using namespace ftxui;

Visualizer::Visualizer(void): amount(3000)
{
	options = Options_t();
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
	PrepareCommands_();

	vector<Result *>	sorted;
	for (auto& res : results)
		sorted.push_back(&res.second);
	
	sort(sorted.begin(), sorted.end(),
		[](const Result *a, const Result *b)
		{
			return a->priority < b->priority;
		});

	size_t	i = 1;
	for (auto *ptr : sorted)
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
		cout << C_MINT "[" C_PURPLE << i << C_MINT "/" C_PURPLE << sorted.size() << C_MINT "] Executing " C_RED C_BOLD << ptr->task << C_RESET << endl;

		try
		{
			ExecuteCommand_(ptr);
		}
		catch(const std::exception& e)
		{
			system("rm -rf ../../.visu_tmp");
			system("cd ../../ && make fclean > /dev/null");
			system("clear");
			cerr << C_RED << ptr->task << ": " << e.what() << C_RESET << endl;
			return;
		}
		
		i++;
	}

	CreateMetrics_();
}

void	Visualizer::ExecuteCommand_(Result* ptr)
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

	if (ptr->task_id == TMP_DIR)
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
	results[TMP_DIR] = Result(TMP_DIR, "cd ../../ && mkdir -p .visu_tmp", 0);
	results[TMP_DIR].tmp_file.clear();
	results[TMP_DIR].tmp_file_err.clear();
	results[MAKE_FCLEAN] = Result(MAKE_FCLEAN, "cd ../../ && make fclean", 1);

	// Compilation
	if (options.selectCompilationFlags != 1)
		results[MAKE] = Result(MAKE, "cd ../../ && make", 2);
	if (options.selectCompilationFlags > 0)
		results[MAKE_BONUS] = Result(MAKE_BONUS, "cd ../../ && make bonus", 2);
	
	// Input generation
	results[GENERATE_INPUT] = Result(GENERATE_INPUT, "cd ../../ && shuf -i 1-100000 -n 3000 | tr '\\n' ' '", 3);

	// Run programs
	if (options.selectedRunMode != 1)
	{
		if (results.find(MAKE) != results.end())
			results[RUN] = Result(RUN, "cd ../../ && ./PmergeMe file:.visu_tmp/" + results[GENERATE_INPUT].tmp_file, 4);
		if (results.find(MAKE_BONUS) != results.end())
			results[RUN_BONUS] = Result(RUN_BONUS, "cd ../../ && ./PmergeMe_with_turbo file:.visu_tmp/" + results[GENERATE_INPUT].tmp_file, 4);
	}
	if (options.selectedRunMode > 0)
	{
		if (results.find(MAKE) != results.end())
			results[VALGRIND_RUN] = Result(VALGRIND_RUN, "cd ../../ && valgrind " VG_FLAGS " ./PmergeMe file:.visu_tmp/" + results[GENERATE_INPUT].tmp_file, 4);
		if (results.find(MAKE_BONUS) != results.end())
			results[VALGRIND_RUN_BONUS] = Result(VALGRIND_RUN_BONUS, "cd ../../ && valgrind " VG_FLAGS " ./PmergeMe_with_turbo file:.visu_tmp/" + results[GENERATE_INPUT].tmp_file, 4);
	}
}

void	Visualizer::CreateMetrics_(void)
{
	const string	wait_message = "Analyzing metrics...";
	jthread	spinner([wait_message](stop_token stoken)
		{
			Utils::LoadingSpinner(stoken, wait_message);
		});

	sleep(10);

	spinner.request_stop();
}
