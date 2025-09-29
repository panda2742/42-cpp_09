#include "Visualizer.hpp"

#include <iostream>
#include <time.h>

int	main(void)
{
	Visualizer	visu;

	srand(time(NULL));

	std::cout << "\033[2J\033[1;1H" << std::flush;
	visu.Launch();

	return 0;
}
