#include "Visualizer.hpp"

#include <iostream>
#include <time.h>

using namespace std;

int	main(void)
{
	Visualizer	visu;

	srand(time(NULL));

	cout << "\033[2J\033[1;1H" << flush;
	visu.Launch();

	return 0;
}
