// Component, Renderer, hbox, text, size, WIDTH, EQUAL, color, Color, separator, xflex
#include "ftxui/component/component.hpp"
#include "InterfaceManager.hpp" // InterfaceManager
#include <string> // string
#include "Utils.hpp" // MAX_VALUE
#include <vector> // vector
#include "ExecutionResult.hpp" // vector

namespace visual
{

using namespace ftxui;

Component	InterfaceManager::Wrap(const std::string& title, Component component)
{
	return Renderer(component, [title, component] {
		return hbox({
			text(title) | size(WIDTH, EQUAL, 25) | color(Color::Orange1),
			separator(),
			component->Render() | xflex,
		}) | xflex;
	});
}

InterfaceManager::InterfaceManager(Visualizer& invoker)
	: invoker_(invoker) {}

Component	InterfaceManager::InitLayout(ScreenInteractive& screen)
{
	std::vector<std::string>	flagsEntries = {
		"Compile without optimization",
		"Compile with optimizations flags",
		"Test both"
	};
	auto	flagsRadiobox = Wrap("1. Compilation flags", Radiobox(&flagsEntries, &invoker_.GetOptions().select_compilation_flags));

	std::vector<std::string>	runModeEntries = {
		"Run normally",
		"Run with Valgrind",
		"Test both"
	};
	auto	runModeRadiobox = Wrap("2. Run mode", Radiobox(&runModeEntries, &invoker_.GetOptions().selected_run_mode));

	auto	input_validator = [&]
	{
		&invoker_.GetOptions().amount_of_elements.erase(
			remove_if(
				invoker_.GetOptions().amount_of_elements.begin(),
				invoker_.GetOptions().amount_of_elements.end(),
				[](char c)
				{
					return !isdigit(c);
				}
			),
			invoker_.GetOptions().amount_of_elements.end()
		);

		if (!invoker_.GetOptions().amount_of_elements.empty())
		{
			try
			{
				invoker_.GetOptions().amount = stoul(invoker_.GetOptions().amount_of_elements);
				if (invoker_.GetOptions().amount > MAX_VALUE)
				{
					invoker_.GetOptions().amount = MAX_VALUE;
					invoker_.GetOptions().amount_of_elements = MAX_VALUE_STR;
				}
			}
			catch(const std::exception& e)
			{
				invoker_.GetOptions().amount = 0;
				invoker_.GetOptions().amount_of_elements = "0";
			}
		}
		else
		{
			invoker_.GetOptions().amount = 0;
			invoker_.GetOptions().amount_of_elements = "0";
		}
	};
	auto	input = Wrap(
		"3. Number of elements",
		Input(&invoker_.GetOptions().amount_of_elements, "Size") | CatchEvent([&](Event event)
			{
				if (event == Event::Return)
				{
					input_validator();
					return true;
				}
				return false;
			})
		);

	auto	action = [&]
	{
		screen.Exit();
	};
	auto	launchButton = Button("LAUNCH", action, ButtonOption::Animated(Color::Orange1));

	auto	layout = Container::Vertical({
		flagsRadiobox,
		runModeRadiobox,
		input,
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
				launchButton->Render() | border,
			}) | xflex | size(WIDTH, GREATER_THAN, 40) | border,
		});
	});
}

Component	InterfaceManager::MetricsLayout(ScreenInteractive& screen)
{
	auto	screen = ScreenInteractive::Fullscreen();

	int						container_selected = 0;
	std::vector<std::string> container_entries;
	auto					results = invoker_.GetExecutionResult().GetResults();
	transform(results.begin(), results.end(), back_inserter(container_entries),
		[](const auto& pair) { return pair.first; }
	);

	auto	menu_component = Menu(&container_entries, &container_selected);
	auto	menu_renderer = Renderer(menu_component, [&]
	{
		return window(text(" Containers ") | color(Color::BlueViolet) | bold, menu_component->Render()  | color(Color::Orange1));
	});
	auto	metrics_renderer = Renderer([&]
	{
		if (!container_entries.empty())
		{
			std::string						container_name = container_entries[container_selected];
			std::vector<ExecutionResult>	res = (*results.find(container_name)).second;
			
			// Create a dynamic layout based on the number of results
			std::vector<Element> result_elements;
			
			for (size_t i = 0; i < res.size(); ++i)
			{
				const auto& result = res[i];

				auto before_sort_status = result.IsSortedBefore() ? 
					text("Sorted") | color(Color::Green) | bold : 
					text("Not sorted") | color(Color::Red) | bold;
					
				auto after_sort_status = result.IsSortedAfter() ? 
					text("Sorted") | color(Color::Green) | bold : 
					text("Not sorted") | color(Color::Red) | bold;
				
				// Build the result card
				auto result_card = vbox({
					// Header with test configuration
					hbox({
						text("Test #" + std::to_string(i + 1)) | bold | color(Color::Orange1),
						text(" | "),
						text("⏺︎ Valgrind  ") | color(result.IsValgrindEnabled() ? Color::Green : Color::Red),
						text("⏺︎ Flags") | color(result.AreFlagsEnabled() ? Color::Green : Color::Red),
					}),
					
					separator(),

					hbox({
						text("Flow: "),
						before_sort_status,
						text(" → "),
						after_sort_status
					}),

					hbox({
						vbox({
							vbox({
								text("Sequence Size") | color(Color::BlueViolet),
								text(utils::FormatNumber(result.GetSequenceSize())) | color(Color::White)
							}) | flex,

							vbox({
								text("Init Time") | color(Color::BlueViolet),
								text(utils::FormatNumber(result.GetInitTime()) + "µs") | color(Color::White)
							}) | flex,
						}) | flex,
						vbox({
							vbox({
								text("Init Threads") | color(Color::BlueViolet),
								text(utils::FormatNumber(result.GetInitThreadsCount())) | color(Color::White)
							}) | flex,

							vbox({
								text("Sort Time") | color(Color::BlueViolet),
								text(utils::FormatNumber(result.GetSortTime()) + "µs") | color(Color::White)
							}) | flex,
						}) | flex,
						vbox({
							vbox({
								text("Sort Threads") | color(Color::BlueViolet),
								text(utils::FormatNumber(result.GetSortThreadsCount())) | color(Color::White)
							}) | flex,

							vbox({
								text("Total Time") | color(Color::BlueViolet),
								text(utils::FormatNumber(result.GetInitTime() + result.GetSortTime()) + "µs") | color(Color::DarkOrange) | bold
							}) | flex
						}) | flex,
					}) | border | color(Color::GrayLight) | flex
				}) | borderRounded | color(Color::BlueViolet) | flex;

				result_elements.push_back(result_card);
			}

			std::vector<Element> rows;
			for (size_t i = 0; i < result_elements.size(); i += 2)
			{
				if (i + 1 < result_elements.size())
				{
					rows.push_back(hbox({
						result_elements[i] | flex,
						result_elements[i + 1] | flex
					}));
				}
				else
				{
					rows.push_back(hbox({
						result_elements[i] | flex,
						text("") | flex
					}));
				}
			}

			if (rows.empty())
			{
				rows.push_back(
					text("No metrics available for " + container_name) | 
					color(Color::Orange1) | bold | center
				);
			}
			
			return window(
				text(" Metrics for " + container_name + " (" + std::to_string(res.size()) + " tests) ") | 
				color(Color::BlueViolet) | bold, 
				vbox(std::move(rows))
			);
		}
		return window(
			text(" No container selected "), 
			text("Please select a container from the left menu") | center | color(Color::Orange1)
		);
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

}
