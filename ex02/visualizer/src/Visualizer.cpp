#include "Visualizer.hpp"
#include <iostream>
#include <vector>
#include "UIStore.hpp"
#include "ui.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace std;
using namespace ftxui;

void	Visualizer::Launch(void) const
{
	ScreenInteractive	screen = ScreenInteractive::TerminalOutput();

	int				selected_tab = 0;
	vector<string>	tab_names = {
		"  Generate input  ",
		"  Start program  ",
		"  Start Valgrind  "
	};

	Component	filename_input = FilenameInputComp(), amount_slider = AmountSliderComp(), amount_input = AmountInputComp();
	Component	generation_view = Container::Vertical({ filename_input, amount_slider, amount_input });
	Component	generation_renderer = Renderer(generation_view, [&] {
		return vbox({
			vbox({
				text("Output filename") | bold,
				text(""),
				filename_input->Render() | flex,
			}) | borderStyled(BorderStyle::EMPTY),
			separator() | color(Color::GrayDark),
			vbox({
				text("Amount of elements") | bold,
				text(""),
				text(FormatNumber(UIStore::use_generation_store.elements_amount_value.value)) | bold | flex | color(Color::Green),
				amount_slider->Render() | flex,
				hbox({
					text("You can also write it here -> "),
					amount_input->Render() | bold,
				}),
			}) | borderStyled(BorderStyle::EMPTY)
		}) | border;
	});
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
		Component	active_tab_content;

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

	screen.Loop(renderer);
}

string	Visualizer::FormatNumber(long long n)
{
	string	s = to_string(n);
	size_t	len = s.length();

	if (len <= 3)
		return s;

	int	first_group = len % 3;
	if (first_group == 0 && len > 0)
		first_group = 3;
	
	string	formatted;
	formatted += s.substr(0, first_group);

	for (size_t	i = first_group; i < len; i += 3)
	{
		formatted += "_";
		formatted += s.substr(i, 3);
	}

	return formatted;
}
