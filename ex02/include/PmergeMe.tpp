#include "PmergeMe.hpp"

#include <iostream>

#include <sys/time.h>
#include "PMMException.hpp"

template <class S>
double	PmergeMe<S>::_GetTimeDiff(timeval & start, timeval & end)
{
	return (end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec - start.tv_usec;
}

template <class S>
PmergeMe<S>::PmergeMe(const char **seq, size_t seq_size) throw(typename S::SortableInvalidElement)
		: __sortable_(new S()), __measure_time_(false)
{
	try
	{
		if (__measure_time_ && gettimeofday(&__tv_init_start_, NULL) == -1)
			__measure_time_ = false;

		__sortable_->Fill(seq, seq_size);

		if (__measure_time_ && gettimeofday(&__tv_init_end_, NULL) == -1)
			__measure_time_ = false;
	
		Display();

		if (__measure_time_)
		{
			double	time_res = _GetTimeDiff(__tv_init_start_, __tv_init_end_);

			std::cout << BLUE_SILVER "Initialization took " RED << time_res << BLUE_SILVER "μs (~" BLUE_SILVER
					<< time_res / 1000000L << BLUE_SILVER "s)." RESET << std::endl;
		}
	}
	catch (const std::exception & e)
	{
		throw;
	}
}

template <class S>
template <class T> PmergeMe<S>::PmergeMe(const PmergeMe<T> & other)
		: __sortable_(new S(other.__sortable_)), __measure_time_(other.__measure_time_) {}

template <class S>
PmergeMe<S>::~PmergeMe(void)
{
	if (__sortable_)
		delete __sortable_;
}

template <class S>
PmergeMe<S> &	PmergeMe<S>::operator=(const PmergeMe<S> & other)
{
	if (this != *other)
	{
		if (this->__sortable_)
			delete this->__sortable_;

		this->__sortable_ = new S(other.__sortable_);
		this->__measure_time_ = other.__measure_time_;
	}

	return *this;
}

template <class S>
void	PmergeMe<S>::FordJohnson(void)
{
	if (__measure_time_ && gettimeofday(&__tv_sort_start_, NULL) == -1)
		__measure_time_ = false;

	__sortable_->Sort();

	if (__measure_time_ && gettimeofday(&__tv_sort_end_, NULL) == -1)
		__measure_time_ = false;
	
	Display();

	if (__measure_time_)
	{
		double	time_res = _GetTimeDiff(__tv_sort_start_, __tv_sort_end_);

		std::cout << BLUE_SILVER "Sorting took " RED << time_res << BLUE_SILVER "μs (~" BLUE_SILVER
				<< time_res / 1000000L << BLUE_SILVER "s)." RESET << std::endl;
	}
}

template <class S>
void	PmergeMe<S>::Display(void) const
{
	const typename S::ContainerType seq = __sortable_->GetSequence();

	std::cout << GREY "Sequence data:" RESET "\n";
	for (typename S::ContainerType::const_iterator	it = seq.begin(); it != seq.end(); it++)
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
	__measure_time_ = true;
}

template <class S>
void	PmergeMe<S>::DisableTimeMeasure(void)
{
	__measure_time_ = false;
}

template <class S>
bool	PmergeMe<S>::IsTimeMeasureEnabled(void) const
{
	return __measure_time_;
}

template <class S>
S	*PmergeMe<S>::GetSortable(void) const
{
	return __sortable_;
}

template <class S>
PmergeMe<S>::PmergeMe(void) throw(PMMException)
		: __sortable_(NULL), __measure_time_(false)
{
	throw PMMException("This class cannot be instanciated without a sequence.");
}