#include "Visualizer.hpp"

#include <iostream>

using namespace std;

int	main(void)
{
	Visualizer	visu;

	cout << "\033[2J\033[1;1H" << flush;
	visu.Launch();

	return 0;
}
