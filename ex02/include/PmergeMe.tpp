#include "PmergeMe.hpp"

#include <iomanip>
#include <iostream>

#include <sys/time.h>
#include "PMMException.hpp"

template <class S>
double	PmergeMe<S>::GetTimeDiff_(timeval & start, timeval & end)
{
	return (end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec - start.tv_usec;
}

template <class S>
bool	PmergeMe<S>::IsSorted_(void) const
{
	const typename S::seq_t seq = sortable_->GetSequence();
	const typename S::seq_t copy = sortable_->GetCopy();

	if (sortable_->GetSequence().size() < 2)
		return true && copy.size() == seq.size();

	for (typename S::const_it_t	it = seq.begin(); (it + 1) != seq.end(); it++)
	{
		if (*it > *(it + 1))
			return false;
	}
	return true && copy.size() == seq.size();
}

template <class S>
PmergeMe<S>::PmergeMe(const char **seq, uint64_t seq_size, const std::string & container_name)
		: sortable_(new S()), measure_time_(false), container_name_(container_name)
{
	EnableTimeMeasure();
	container_name_ = container_name;
	try
	{
		if (measure_time_ && gettimeofday(&tv_init_start_, NULL) == -1)
			measure_time_ = false;

		sortable_->Fill(seq, seq_size);

		if (measure_time_ && gettimeofday(&tv_init_end_, NULL) == -1)
			measure_time_ = false;

		Display();

		if (measure_time_)
		{
			double	time_res = GetTimeDiff_(tv_init_start_, tv_init_end_);

			std::cout << std::fixed << std::setprecision(3);
			std::cout << "[" << container_name_ << "] Initialization: " << time_res << "μs (~" << time_res / 1000L
				<< "ms, ~" << time_res / 1000000L << "s)." << std::endl;
		}
	}
	catch (const std::exception & e)
	{
		delete sortable_;
		throw;
	}
}

template <class S>
template <class T> PmergeMe<S>::PmergeMe(const PmergeMe<T> & other)
		: sortable_(new S(other.sortable_)), measure_time_(other.measure_time_), container_name_(other.container_name_) {}

template <class S>
PmergeMe<S>::~PmergeMe(void)
{
	if (sortable_)
		delete sortable_;
}

template <class S>
PmergeMe<S> &	PmergeMe<S>::operator=(const PmergeMe<S> & other)
{
	if (this != *other)
	{
		if (this->sortable_)
			delete this->sortable_;

		this->container_name_ = other.container_name_;
		this->sortable_ = new S(other.sortable_);
		this->measure_time_ = other.measure_time_;
	}

	return *this;
}

template <class S>
void	PmergeMe<S>::FordJohnson(void)
{
	if (measure_time_ && gettimeofday(&tv_sort_start_, NULL) == -1)
		measure_time_ = false;

	sortable_->Sort();

	if (measure_time_ && gettimeofday(&tv_sort_end_, NULL) == -1)
		measure_time_ = false;
	
	Display();

	if (measure_time_)
	{
		double	time_res = GetTimeDiff_(tv_sort_start_, tv_sort_end_);

		std::cout << std::fixed << std::setprecision(3);
		std::cout << "[" << container_name_ << "] Sorting: " << time_res << "μs (~" << time_res / 1000L
			<< "ms, ~" << time_res / 1000000L << "s)." << std::endl;
	}
}

template <class S>
void	PmergeMe<S>::Display(void) const
{
	const typename S::seq_t seq = sortable_->GetSequence();

	std::cout << "Sequence data: " << (IsSorted_() ? GREEN "sorted" : RED "not sorted");
	std::cout << RESET "\n";
	for (typename S::const_it_t	it = seq.begin(); it != seq.end(); it++)
	{
		if (it != seq.begin())
			std::cout << "  ";
		std::cout << *it;
	}
	std::cout << std::endl;
}

template <class S>
void	PmergeMe<S>::EnableTimeMeasure(void)
{
	measure_time_ = true;
}

template <class S>
void	PmergeMe<S>::DisableTimeMeasure(void)
{
	measure_time_ = false;
}

template <class S>
bool	PmergeMe<S>::IsTimeMeasureEnabled(void) const
{
	return measure_time_;
}

template <class S>
S	*PmergeMe<S>::GetSortable(void) const
{
	return sortable_;
}

template <class S>
PmergeMe<S>::PmergeMe(void)
		: sortable_(NULL), measure_time_(false), container_name_("")
{
	throw PMMException("This class cannot be instanciated without a sequence.");
}