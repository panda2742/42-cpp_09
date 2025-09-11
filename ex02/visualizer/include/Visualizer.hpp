#pragma once

#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#define MAX_VALUE 2000000
#define MAX_VALUE_STR "2000000"

using namespace std;
using namespace ftxui;

class Visualizer
{
	public:
		explicit Visualizer(void);
		~Visualizer(void) = default;

		Visualizer(const Visualizer&) = delete;
		Visualizer(Visualizer&&) = delete;

		Visualizer&	operator=(const Visualizer&) = delete;
		Visualizer&	operator=(Visualizer&&) = delete;
	
		void				Launch(void);
		ScreenInteractive&	GetScreen(void);
		static string		FormatNumber(long long n);
	
	private:
		ScreenInteractive	screen_;
};
