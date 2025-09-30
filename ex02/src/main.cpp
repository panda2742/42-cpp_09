#include "PmergeMe.hpp"

#include <iostream>
#include <iterator>
#include <fstream>
#include <list>
#include <vector>

template <template <typename T, typename Alloc> class Ctn>
static void	__TestContainer(
	int argc, char **argv,
	const std::string & container_name
)
{
	try
	{
		PmergeMe<Ctn>	awesome(const_cast<const char **>(argv), static_cast<uint64_t>(argc), container_name);
		awesome.EnableTimeMeasure();
		awesome.FordJohnson();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
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

	__TestContainer<std::deque>(argc, tokens, "deque");
	__TestContainer<std::vector>(argc, tokens, "vector");
	__TestContainer<std::list>(argc, tokens, "list");

	if (allocated)
	{
		for (int	i = 0; i < argc; ++i)
			delete[] tokens[i];
		delete[] tokens;
	}
}
