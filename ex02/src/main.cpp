#include "PmergeMe.hpp"

#include <iostream>

#include "sort/SortableDeque.hpp"
#include "sort/SortableList.hpp"
#include "sort/SortableVector.hpp"

#define BLUE "\e[38;2;0;0;255m"
#define PURPLE "\e[38;2;255;0;255m"
#define YELLOW "\e[38;2;255;255;0m"
#define GREEN "\e[38;2;0;255;0m"

template <class Ctn>
static void	__TestContainer(
	int argc, char **argv,
	const std::string & container_name,
	const std::string & container_color
)
{
	{
		std::cout << "\n" << container_color << "[  Test with " << container_name << ", " << argc << " elements  ]"
				<< RESET "\n" << std::endl;
		try
		{
			PmergeMe<Ctn>	awesome(const_cast<const char **>(argv), static_cast<uint64_t>(argc));
			awesome.EnableTimeMeasure();
			awesome.FordJohnson();
		}
		catch (const std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
		std::cout << RESET "\n" << std::endl;
	}
}

int	main(int argc, char **argv)
{
	argv++;
	argc--;
	__TestContainer<SortableDeque>(argc, argv, "Deque", BLUE);
	// __TestContainer<ListSortable>(argc, argv, "List", YELLOW);
	// __TestContainer<VectorSortable>(argc, argv, "Vector", PURPLE);
}
