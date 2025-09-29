#pragma once

#include "ftxui/component/component.hpp" // Component
#include <string> // string
#include "Visualizer.hpp" // Visualizer

namespace visual
{
	class InterfaceManager
	{
	public:
		static ftxui::Component	Wrap(const std::string&, ftxui::Component);

		InterfaceManager(Visualizer&);
		~InterfaceManager(void) = default;
		InterfaceManager(const InterfaceManager&) = default;
		InterfaceManager(InterfaceManager&&) = default;
		InterfaceManager&	operator=(const InterfaceManager&) = default;
		InterfaceManager&	operator=(InterfaceManager&&) = default;

		ftxui::Component	InitLayout(ftxui::ScreenInteractive& screen);
		ftxui::Component	MetricsLayout(ftxui::ScreenInteractive& screen);

	private:
		Visualizer&			invoker_;
	};
}
