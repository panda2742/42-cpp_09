#include "Visualizer.hpp"
#include <iostream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace std;
using namespace ftxui;

void	Visualizer::Launch(void) const
{
	auto		screen = ScreenInteractive::TerminalOutput();
	Component	base = Container::Vertical({});

	Element	document = hbox({
		text(" CPP Module 09 ") | border,
		text(" Performance Visualizer 📈 ") | border | flex
	});

	auto	renderer = Renderer(base, [&] {
		return document;
	});

	renderer |= CatchEvent([&](Event event) {
		if (event.is_mouse() && event.mouse().button == Mouse::Button::Right)
		{
			screen.ExitLoopClosure()();
			return true;
		}
		return false;
	});

	screen.Loop(renderer);
}
