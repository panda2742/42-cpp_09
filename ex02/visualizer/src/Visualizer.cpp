#include "Visualizer.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace std;
using namespace ftxui;

void	Visualizer::Launch(void) const
{
	auto	screen = ScreenInteractive::TerminalOutput();

	std::cout << "Welcome to the performance visualizer! What would you like to do?" << std::endl;

	int						selected = 0;
	const vector<string>	entries = {
		"Generate a large input.",
		"Launch the program.",
		"Launch the program with turbo mode."
	};

	MenuOption	option;
	option.on_enter = screen.ExitLoopClosure();
	auto		menu = Menu(&entries, &selected, option);

	screen.Loop(menu);

	std::cout << "Selected element: " << entries[selected] << std::endl;
}
