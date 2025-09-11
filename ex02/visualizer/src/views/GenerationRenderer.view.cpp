#include "UI.hpp"

using namespace ftxui;

Component	GenerationRendererView(const Visualizer& visualizer)
{
	Component	filename_input = FilenameInputComp(),
				amount_slider = AmountSliderComp(),
				amount_input = AmountInputComp();

	Component	generation_view = Container::Vertical({ filename_input, amount_slider, amount_input });

	return Renderer(generation_view, [&] {
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
				text(visualizer.FormatNumber(UIStore::use_generation_store.elements_amount_value.value)) | bold | flex | color(Color::Green),
				amount_slider->Render() | flex,
				hbox({
					text("You can also write it here -> "),
					amount_input->Render() | bold,
				}),
			}) | borderStyled(BorderStyle::EMPTY)
		}) | border;
	});
}
