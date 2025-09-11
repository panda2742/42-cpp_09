#include "UI.hpp"
#include <vector>

using namespace ftxui;

Component	LaunchView(const Visualizer& visualizer)
{
	int				selected_tab = 0;
	vector<string>	tab_names = {
		"  Generate input  ",
		"  Start program  ",
		"  Start Valgrind  "
	};

	Component	generation_renderer = GenerationRendererView(visualizer);
	Component	program_renderer = Renderer([&] { return text("Content 2"); });
	Component	valgrind_renderer = Renderer([&] { return text("Content 3"); });

	Component	tab_menu = Menu(&tab_names, &selected_tab, MenuOption::HorizontalAnimated());
	Component	tab_container = Container::Tab({
		generation_renderer, program_renderer, valgrind_renderer
	}, &selected_tab);

	Component	quit_button = Button("[ Quit ]", screen.ExitLoopClosure(), ButtonOption::Animated(Color::Red3));
	Component	container = Container::Vertical({
		quit_button,
		tab_menu,
		tab_container
	});

	Component	renderer = Renderer(container, [&] {
		return vbox({
			hbox({
				text(" CPP Module 09 ") | border,
				text("  [ 📈 ]  PERFORMANCE VISUALIZER         ") | bold | flex | border,
				text(" Made with <3 by ehosta "),
				filler(),
				quit_button->Render(),
            }),
			tab_menu->Render(),
			tab_container->Render() | flex,
		});
	});
}
