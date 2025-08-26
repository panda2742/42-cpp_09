#include "PmergeMe.hpp"

#include <iostream>
#include <iterator>
#include <fstream>

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
	std::cout << "\n" << container_color << "[  Test with " << container_name << ", " << argc << " elements  ]"
			<< RESET "\n" << std::endl;
	try
	{
		PmergeMe<Ctn>	awesome(const_cast<const char **>(argv), static_cast<uint64_t>(argc), "Std::" + container_name);
		awesome.EnableTimeMeasure();
		// awesome.FordJohnson();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << RESET "\n" << std::endl;
}

int	main(int argc, char **argv)
{
	argv++;
	argc--;
	char	**tokens = argv;
	bool	allocated = false;

	if (std::string(argv[0]).substr(0, 5) == "file:")
	{
		std::ifstream	file(std::string(argv[0]).substr(5).c_str());
		if (!file)
		{
			std::cout << RED "Wrong usage: either a sequence or a file:<filename> argument is expected." RESET
				<< std::endl;
			return 1;
		}

		std::istream_iterator<std::string>	begin(file);
		std::istream_iterator<std::string>	end;
		std::vector<std::string>			words(begin, end);

		argc = words.size();
		if ((unsigned int)argc >= UINT_MAX)
		{
			std::cout << RED "Wrong usage: either a sequence or a file:<filename> argument is expected." RESET
				<< std::endl;
			return 1;
		}

		tokens = new char*[argc];
		allocated = true;

		for (int	i = 0; i < argc; ++i)
		{
			tokens[i] = new char[words[i].size() + 1];
			std::strcpy(tokens[i], words[i].c_str());
		}
	}
	if ((unsigned int)argc >= UINT_MAX)
	{
		std::cout << RED "Wrong usage: either a sequence or a file:<filename> argument is expected." RESET
			<< std::endl;
		return 1;
	}

	__TestContainer<SortableDeque>(argc, tokens, "Deque", BLUE);
	// __TestContainer<ListSortable>(argc, argv, "List", YELLOW);
	// __TestContainer<VectorSortable>(argc, argv, "Vector", PURPLE);

	if (allocated)
	{
		for (int	i = 0; i < argc; ++i)
			delete[] tokens[i];
		delete[] tokens;
	}
}
