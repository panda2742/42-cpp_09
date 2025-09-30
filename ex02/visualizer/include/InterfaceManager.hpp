#pragma once

#include "ftxui/component/component.hpp" // Component
#include "ftxui/component/screen_interactive.hpp" // Component
#include <string> // string
#include "VisualizerOptions.hpp"

namespace visual
{
	class ExecutionResultManager;

	class InterfaceManager
	{
	public:
		static ftxui::Component	Wrap(const std::string&, ftxui::Component);

		InterfaceManager(void) = default;
		~InterfaceManager(void) = default;
		InterfaceManager(const InterfaceManager&) = default;
		InterfaceManager(InterfaceManager&&) = default;

		ftxui::Component	InitLayout(VisualizerOptions&);
		ftxui::Component	MetricsLayout(ExecutionResultManager&);
	};
}
