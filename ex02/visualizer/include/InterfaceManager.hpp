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

		InterfaceManager(const Visualizer&);
		~InterfaceManager(void) = default;
		InterfaceManager(const InterfaceManager&) = default;
		InterfaceManager(InterfaceManager&&) = delete;
		InterfaceManager&	operator=(const InterfaceManager&) = default;
		InterfaceManager&	operator=(InterfaceManager&&) = delete;

	private:
		const InterfaceManager&	invoker_;
	};
}
