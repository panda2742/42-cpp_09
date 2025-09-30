#pragma once

#include <string> // string

namespace visual
{
	class VisualizerOptions
	{
	public:
		int			selected_run_mode;
		int			select_compilation_flags;
		int			amount;
		std::string	amount_of_elements;

		VisualizerOptions(void);
		~VisualizerOptions(void) = default;
		VisualizerOptions(const VisualizerOptions&) = default;
		VisualizerOptions(VisualizerOptions&&) = default;
		VisualizerOptions&	operator=(const VisualizerOptions&) = default;
		VisualizerOptions&	operator=(VisualizerOptions&&) = default;
	};
}
