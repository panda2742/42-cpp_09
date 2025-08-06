#include "PmergeMe.hpp"

#include <iostream>

#define BLUE "\e[38;2;0;0;255m"
#define PURPLE "\e[38;2;255;0;255m"
#define YELLOW "\e[38;2;255;255;0m"
#define GREEN "\e[38;2;0;255;0m"

static void	_filling_time(double t);
static void	_sorting_time(double t);

int	main(int argc, char **argv)
{
	argv++;
	argc--;
	try
	{
		std::cout << "\n" BLUE "[ Test with a deque ]" RESET "\n" << std::endl;

		PmergeMe<PMMDeque, PMMDequePair>	awesome;
		_filling_time(awesome.Fill(
			const_cast<const char **>(argv), static_cast<size_t>(argc)
		));
		awesome.Display();
		_sorting_time(awesome.Sort());
		awesome.Display();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	try
	{
		std::cout << "\n" PURPLE "[ Test with a vector ]" RESET "\n" << std::endl;

		PmergeMe<PMMVector, PMMVectorPair>	awesome;
		_filling_time(awesome.Fill(
			const_cast<const char **>(argv), static_cast<size_t>(argc)
		));
		awesome.Display();
		_sorting_time(awesome.Sort());
		awesome.Display();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	try
	{
		std::cout << "\n" YELLOW "[ Test with a list ]" RESET "\n" << std::endl;

		PmergeMe<PMMList, PMMDListPair>	awesome;
		_filling_time(awesome.Fill(
			const_cast<const char **>(argv), static_cast<size_t>(argc)
		));
		awesome.Display();
		_sorting_time(awesome.Sort());
		awesome.Display();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "===============================================" << std::endl;
}

static void	_filling_time(double t)
{
	std::cout << GREEN "Filling time is: " << t << " seconds." RESET
		<< std::endl;
}

static void	_sorting_time(double t)
{
	std::cout << GREEN "Sorting time is: " << t << " seconds." RESET
		<< std::endl;
}
