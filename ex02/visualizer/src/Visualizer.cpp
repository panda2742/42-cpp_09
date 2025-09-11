#include "Visualizer.hpp"
#include <iostream>
#include <vector>
#include "UI.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace std;
using namespace ftxui;

void	Visualizer::Launch(void) const
{
	ScreenInteractive	screen = ScreenInteractive::TerminalOutput();


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
