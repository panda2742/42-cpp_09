// Component, Renderer, hbox, text, size, WIDTH, EQUAL, color, Color, separator, xflex
#include "ftxui/component/component.hpp"
#include "InterfaceManager.hpp" // InterfaceManager
#include <string> // string

namespace visual
{
namespace ui
{

using namespace ftxui;

Component	Wrap(const std::string& title, Component component)
{
	return Renderer(component, [title, component] {
		return hbox({
			text(title) | size(WIDTH, EQUAL, 25) | color(Color::Orange1),
			separator(),
			component->Render() | xflex,
		}) | xflex;
	});
}

}
}
