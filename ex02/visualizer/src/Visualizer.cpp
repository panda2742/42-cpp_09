#include "Visualizer.hpp"
#include <iostream>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace std;
using namespace ftxui;

void	Visualizer::Launch(void) const
{
	// Screen
	ScreenInteractive	screen = ScreenInteractive::TerminalOutput();

	// Tabs
	int				selected_tab = 0;
	vector<string>	tab_names = {
		"  Generate input  ",
		"  Start program  ",
		"  Start Valgrind  "
	};

	static string	output_filename;
	Component		filename_input = Input(&output_filename, "example: .large:1000");

	static uint32_t	amount_slider_value = 100000;
	static string	amount_input_value = "";

	auto	validate_and_sync_input = [&] {
		amount_input_value.erase(
			remove_if(
				amount_input_value.begin(), amount_input_value.end(),
				[](char c) { return !isdigit(c); }
			),
			amount_input_value.end()
		);

		if (!amount_input_value.empty())
		{
			try
			{
				amount_slider_value = stoul(amount_input_value);
				if (amount_slider_value > MAX_VALUE)
				{
					amount_slider_value = MAX_VALUE;
					amount_input_value = MAX_VALUE_STR;
				}
			}
			catch(const std::exception& e)
			{
				amount_slider_value = 0;
				amount_input_value = "0";
			}
		}
		else
		{
			amount_slider_value = 0;
			amount_input_value = "0";
		}
	};

	Component		amount_slider = Slider("", reinterpret_cast<int *>(&amount_slider_value), 0, MAX_VALUE, 1000);
	Component		amount_input = Input(&amount_input_value, "...");

	amount_input |= CatchEvent([&](Event event) {
		if (event == Event::Return)
		{
			validate_and_sync_input();
			return true;
		}
		return false;
	});

	Component	generation_view = Container::Vertical({ filename_input, amount_slider, amount_input });
	Component	generation_renderer = Renderer(generation_view, [&] {
		return vbox({
			hbox({
				text("— Output filename: "),
				filename_input->Render() | flex,
			}),
			hbox({
				text("— Amount of values: "),
				text(FormatNumber(amount_slider_value)) | bold,
			}),
			hbox({
				amount_slider->Render() | flex,
				text("  write the amount -> "),
				amount_input->Render() | bold,
			}),
		});
	});

	Component	program_renderer = Renderer([&] { return text("Content 2"); });
	Component	valgrind_renderer = Renderer([&] { return text("Content 3"); });

	Component	tab_menu = Menu(&tab_names, &selected_tab, MenuOption::HorizontalAnimated());
	Component	tab_container = Container::Tab({
		generation_renderer, program_renderer, valgrind_renderer
	}, &selected_tab);

	// Header
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
