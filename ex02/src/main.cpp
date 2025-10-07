#include "PmergeMe.hpp"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <deque>
#include <stdint.h>
#include <sstream>

#define TESTS_PER_CONTAINER 10

static char	**_compute_sequence(int *argc, char **argv, bool *allocated);
template <template <class T, class Alloc> class Ctn>
static void	_generate_tests(const char **seq, uint32_t seq_size, const std::string& ctn_name);

int	main(int argc, char **argv)
{
	argv++;
	argc--;
	bool	allocated = false;

	char	**tokens = _compute_sequence(&argc, argv, &allocated);

	try
	{
		_generate_tests<std::vector>(const_cast<const char **>(tokens), static_cast<uint32_t>(argc), "vector");
		_generate_tests<std::deque>(const_cast<const char **>(tokens), static_cast<uint32_t>(argc), "deque");
	}
	catch (const std::exception & e) { std::cout << e.what() << std::endl; }

	if (allocated)
	{
		for (int	i = 0; i < argc; ++i)
			delete[] tokens[i];
		delete[] tokens;
	}
}

static char	**_compute_sequence(int *argc, char **argv, bool *allocated)
{
	if ((unsigned int)*argc >= 0xffffffff || *argc == 0)
	{
		std::cout << "Wrong usage: either a sequence or a file:<filename> argument is expected."
			<< std::endl;
		return NULL;
	}

	char	**tokens = argv;
	if (std::string(argv[0]).substr(0, 5) == "file:")
	{
		std::ifstream	file(std::string(argv[0]).substr(5).c_str());
		if (!file)
		{
			std::cout << "Wrong usage: either a sequence or a file:<filename> argument is expected."
				<< std::endl;
			return NULL;
		}

		std::istream_iterator<std::string>	begin(file);
		std::istream_iterator<std::string>	end;
		std::vector<std::string>			words(begin, end);

		*argc = words.size();
		if ((unsigned int)*argc >= 0xffffffff)
		{
			std::cout << "Wrong usage: either a sequence or a file:<filename> argument is expected."
				<< std::endl;
			return NULL;
		}

		tokens = new char*[*argc];
		*allocated = true;

		for (int	i = 0; i < *argc; ++i)
		{
			tokens[i] = new char[words[i].size() + 1];
			std::strcpy(tokens[i], words[i].c_str());
		}
	}
	return tokens;
}

template <template <typename T, typename Alloc> class Ctn>
static void	_generate_tests(const char **seq, uint32_t seq_size, const std::string& ctn_name)
{
	for (unsigned int	i = 0; i < TESTS_PER_CONTAINER; ++i)
	{
		std::stringstream	ss;
		ss << ctn_name << " " << std::setbase(16) << i;

		PmergeMe<Ctn>	vec(seq, seq_size, ss.str());
	}
}
