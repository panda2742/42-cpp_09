#include "UI.hpp"
#include <vector>
#include <string>
#include <memory>

using namespace ftxui;

Component	LaunchView(Visualizer& visualizer)
{
	Component	generation_renderer = GenerationRendererView(visualizer);
	Component	program_renderer = Renderer([] { return text("Content 2"); });
	Component	valgrind_renderer = Renderer([] { return text("Content 3"); });

	Component	tab_menu = Menu(&UIStore::use_main_view_store.tab_names.value, &UIStore::use_main_view_store.selected_tab.value, MenuOption::HorizontalAnimated());
	Component	tab_container = Container::Tab({
		generation_renderer, program_renderer, valgrind_renderer
	}, &UIStore::use_main_view_store.selected_tab.value);

	Component	quit_button = Button("[ Quit ]", [screen = &visualizer.GetScreen()] {screen->ExitLoopClosure();}, ButtonOption::Animated(Color::Red3));
	Component	container = Container::Vertical({
		quit_button,
		tab_menu,
		tab_container
	});

	return Renderer(container, [&] {
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
