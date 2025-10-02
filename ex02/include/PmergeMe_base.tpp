#include "PmergeMe.hpp"
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sys/time.h>

// ===========================================================
// |                     PUBLIC MEMBERS                      |
// ===========================================================

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn>::PmergeMe(const char **seq, uint32_t seq_size, const std::string & container_name)
		: container_name_(container_name)
{
	std::cout << "START|container:" << container_name_ << "|size:" << seq_size <<std::endl;
	fill_args_t	args = { .seq = seq, .seq_size = seq_size };
	chrono_("time_init", &PmergeMe<Ctn>::fill_, &args);
	display_();
	chrono_("time_sort", &PmergeMe<Ctn>::ford_johnson_, NULL);
	display_();
	std::cout << "END|container:" << container_name << std::endl;
}

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn>::PmergeMe(const PmergeMe<Ctn>&) {}

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn>::~PmergeMe(void) {}

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn> &	PmergeMe<Ctn>::operator=(const PmergeMe<Ctn>&) { return *this; }
