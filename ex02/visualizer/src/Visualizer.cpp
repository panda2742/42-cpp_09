#include "Visualizer.hpp"

#include <array>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <thread>

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/screen.hpp"

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
	auto	launchButton = Button("Launch", action, ButtonOption::Animated(Color::Purple));

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
			}) | xflex | size(WIDTH, GREATER_THAN, 40) | border,
			launchButton->Render(),
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
	map<string, string>	tasks;
	tasks["init"] = "cd ../../ && make fclean";
	if (options.selectCompilationFlags != 1)
		tasks["normal_compile"] = "cd ../../ && make";
	if (options.selectCompilationFlags > 0)
		tasks["valgrind_compile"] = "cd ../../ && make bonus";

	size_t	tasks_size = tasks.size();
	string	complete = "";
	float	i = 0;
	string	reset_pos;
	for (auto 	t : tasks)
	{
		string	tmp_filename = ".tmp_" + to_string(reinterpret_cast<unsigned long long>(&t));
		complete += string("\n`") + t.first + "`: " + t.second;

		auto	document = vbox({
			paragraph(complete) | bold,
			hbox({
				text(to_string(static_cast<int>(i)) + "/" + to_string(tasks_size) + " "),
				vbox({
					gauge(i / static_cast<float>(tasks_size)) | flex,
				}) | size(WIDTH, EQUAL, 100) | border,
			}),
		});
		auto	screen = Screen(100, 3);
		Render(screen, document);
		cout << reset_pos;
		screen.Print();
		reset_pos = screen.ResetPosition();

		array<char, 128>	buffer;
		string				result;
		unique_ptr			<FILE, decltype(&pclose)>	pipe(popen(string(t.second + " &> " + tmp_filename).c_str(), "r"), pclose);

		if (!pipe)
			continue;
		while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
			result += buffer.data();

		this_thread::sleep_for(0.1s);
		++i;
	}

	auto	document = vbox({
		paragraph(complete) | bold,
		hbox({
			text(to_string(static_cast<int>(i)) + "/" + to_string(tasks_size) + " "),
			vbox({
				gauge(i / static_cast<float>(tasks_size)) | flex,
			}) | size(WIDTH, EQUAL, 100) | border,
		}),
	});
	auto	screen = Screen(100, 3);
	Render(screen, document);
	cout << reset_pos;
	screen.Print();
	reset_pos = screen.ResetPosition();
}

