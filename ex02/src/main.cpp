#include "PmergeMe.hpp"

#include <iostream>

#include "sort/DequeSortable.hpp"
#include "sort/ListSortable.hpp"
#include "sort/VectorSortable.hpp"

#define BLUE "\e[38;2;0;0;255m"
#define PURPLE "\e[38;2;255;0;255m"
#define YELLOW "\e[38;2;255;255;0m"
#define GREEN "\e[38;2;0;255;0m"

int	main(int argc, char **argv)
{
	argv++;
	argc--;
	try
	{
		std::cout << "\n" BLUE "[ Test with a deque ]" RESET "\n" << std::endl;

		PmergeMe<DequeSortable>	awesome(const_cast<const char **>(argv), static_cast<size_t>(argc));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	try
	{
		std::cout << "\n" YELLOW "[ Test with a list ]" RESET "\n" << std::endl;

		PmergeMe<ListSortable>	awesome(const_cast<const char **>(argv), static_cast<size_t>(argc));
		awesome.Display();
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	try
	{
		std::cout << "\n" PURPLE "[ Test with a vector ]" RESET "\n" << std::endl;

		PmergeMe<VectorSortable>	awesome(const_cast<const char **>(argv), static_cast<size_t>(argc));
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "===============================================" << std::endl;
}
