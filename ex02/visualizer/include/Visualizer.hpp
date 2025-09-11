#pragma once

#include <string>

#include <ftxui/component/component.hpp>

#define MAX_VALUE 2000000
#define MAX_VALUE_STR "2000000"

using namespace std;
using namespace ftxui;

class Visualizer
{
	public:
		explicit Visualizer(void) = default;
		~Visualizer(void) = default;

		Visualizer(const Visualizer&) = delete;
		Visualizer(Visualizer&&) = default;

		Visualizer&	operator=(const Visualizer&) = delete;
		Visualizer&	operator=(Visualizer&&) = default;
	
		void			Launch(void) const;
		static string	FormatNumber(long long n);
};
