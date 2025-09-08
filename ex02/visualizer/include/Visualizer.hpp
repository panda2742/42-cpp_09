#pragma once

#include <string>

using namespace std;

class Visualizer
{
	public:
		explicit Visualizer(void) = default;
		~Visualizer(void) = default;

		Visualizer(const Visualizer&) = delete;
		Visualizer(Visualizer&&) = default;

		Visualizer&	operator=(const Visualizer&) = delete;
		Visualizer&	operator=(Visualizer&&) = default;
	
		void	Launch(void) const;
};
