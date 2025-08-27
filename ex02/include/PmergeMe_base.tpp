#include "PmergeMe.hpp"
#include "Algorithm.hpp"
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sys/time.h>

// ===========================================================
// |                     PUBLIC MEMBERS                      |
// ===========================================================

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn>::PmergeMe(const char **seq, uint64_t seq_size, const std::string & container_name)
		: sequence_(), copy_(), measure_time_(false), container_name_(container_name)
{
	std::cout << "START|container:" << container_name_ << "|size|" << seq_size <<std::endl;
	EnableTimeMeasure();
	container_name_ = container_name;
	try
	{
		if (measure_time_ && gettimeofday(&tv_init_start_, NULL) == -1)
			measure_time_ = false;

		Fill_(seq, seq_size);

		if (measure_time_ && gettimeofday(&tv_init_end_, NULL) == -1)
			measure_time_ = false;

		Display();

		if (measure_time_)
		{
			double	time_res = GetTimeDiff_(tv_init_start_, tv_init_end_);

			std::cout << std::fixed << std::setprecision(3);
			std::cout << "container:" << container_name_ << "timer|init|" << time_res << std::endl;
		}
	}
	catch (const std::exception & e)
	{
		throw;
	}
}

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn>::~PmergeMe(void) {}

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn> &	PmergeMe<Ctn>::operator=(const PmergeMe<Ctn> & other)
{
	if (this != *other)
	{
		if (this->sortable_)
			delete this->sortable_;

		this->container_name_ = other.container_name_;
		this->measure_time_ = other.measure_time_;
	}

	return *this;
}

// ===========================================================
// |                     PRIVATE MEMBERS                     |
// ===========================================================

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn>::PmergeMe(void)
		: sequence_(), copy_(), measure_time_(false), container_name_("")
{
	throw std::runtime_error("This class cannot be instanciated without a sequence.");
}

template <template <class T, class Alloc> class Ctn>
PmergeMe<Ctn>::PmergeMe(const PmergeMe<Ctn> & other)
		: sequence_(), copy_(), measure_time_(other.measure_time_), container_name_(other.container_name_)
{
	throw std::runtime_error("This class cannot be instanciated without a sequence.");
}

