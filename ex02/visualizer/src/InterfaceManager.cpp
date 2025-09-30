#include "ftxui/component/component.hpp"
#include "InterfaceManager.hpp"
#include <string>
#include "Utils.hpp"
#include <vector>
#include "ExecutionResult.hpp"
#include "ExecutionResultManager.hpp"

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

void	InterfaceManager::InitLayout(VisualizerOptions& options)
{
	auto	screen = ScreenInteractive::Fullscreen();

	std::vector<std::string>	flagsEntries = {
		"Compile without optimization",
		"Compile with optimizations flags",
		"Test both"
	};
	auto	flagsRadiobox = Wrap("1. Compilation flags", Radiobox(&flagsEntries, &options.select_compilation_flags));

	std::vector<std::string>	runModeEntries = {
		"Run normally",
		"Run with Valgrind",
		"Test both"
	};
	auto	runModeRadiobox = Wrap("2. Run mode", Radiobox(&runModeEntries, &options.selected_run_mode));

	auto	input_validator = [&]
	{
		options.amount_of_elements.erase(
			remove_if(
				options.amount_of_elements.begin(),
				options.amount_of_elements.end(),
				[](char c)
				{
					return !isdigit(c);
				}
			),
			options.amount_of_elements.end()
		);

		if (!options.amount_of_elements.empty())
		{
			try
			{
				options.amount = stoul(options.amount_of_elements);
				if (options.amount > MAX_VALUE)
				{
					options.amount = MAX_VALUE;
					options.amount_of_elements = MAX_VALUE_STR;
				}
			}
			catch(const std::exception& e)
			{
				options.amount = 0;
				options.amount_of_elements = "0";
			}
		}
		else
		{
			options.amount = 0;
			options.amount_of_elements = "0";
		}
	};
	auto	input = Wrap(
		"3. Number of elements",
		Input(&options.amount_of_elements, "Size") | CatchEvent([&](Event event)
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

	screen.Loop(layoutComponent);
}

void	InterfaceManager::MetricsLayout(ExecutionResultManager& execution_result_manager)
{
	auto	screen = ScreenInteractive::Fullscreen();

	int						container_selected = 0;
	std::vector<std::string> container_entries;
	auto					results = execution_result_manager.GetResults();
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

			std::vector<Element> result_elements;

			for (size_t i = 0; i < res.size(); ++i)
			{
				const auto& result = res[i];

				auto	before_sort_status = result.IsSortedBefore() ?
					text("Sorted") | color(Color::Green) | bold :
					text("Not sorted") | color(Color::Red) | bold;

				auto	after_sort_status = result.IsSortedAfter() ?
					text("Sorted") | color(Color::Green) | bold :
					text("Not sorted") | color(Color::Red) | bold;

				auto	errors_summary = result.GetErrorsSummary()[0] > 0 ?
					text(utils::FormatNumber(result.GetErrorsSummary()[0])) | color(Color::Red) | bold :
					text("0") | color(Color::Green) | bold ;

				auto	context_summary = result.GetErrorsSummary()[0] > 0 ?
					text(utils::FormatNumber(result.GetErrorsSummary()[1])) | color(Color::Red) | bold :
					text("0") | color(Color::Green) | bold ;

				auto result_card = vbox({
					hbox({
						text("Test #" + std::to_string(i + 1)) | bold | color(Color::Orange1),
						text(" | "),
						text("⏺︎ Valgrind  ") | color(result.IsValgrindEnabled() ? Color::Green : Color::Red),
						text("⏺︎ Flags") | color(result.AreFlagsEnabled() ? Color::Green : Color::Red),
					}),

					separator(),

					hbox({
						hbox({
							text("Flow: "),
							before_sort_status,
							text(" → ") | color(Color::White),
							after_sort_status,
						}) | flex,
						hbox({
							text("Errors: "),
							errors_summary,
							text(" in ") | color(Color::White),
							context_summary,
							text(" contexts."),
						}) | flex
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
					}) | border | color(Color::GrayLight) | flex,

					hbox({
						vbox({
							text("Allocations") | color(Color::BlueViolet),
							text("Frees") | color(Color::BlueViolet),
							text("Allocated bytes") | color(Color::BlueViolet),
						}) | flex,
						vbox({
							text(utils::FormatNumber(result.GetHeapSummary()[0])) | color(Color::DarkOrange),
							text(utils::FormatNumber(result.GetHeapSummary()[1])) | color(Color::DarkOrange),
							text(utils::FormatNumber(result.GetHeapSummary()[2]) + " bytes") | color(Color::White)
						}) | flex,
						vbox({
							text("                    "),
							text("                    "),
							text("                    "),
						}) | flex,
					}) | border | color(Color::GrayLight) | flex,
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
