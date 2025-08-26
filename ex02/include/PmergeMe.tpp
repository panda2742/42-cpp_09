#include "PmergeMe.hpp"

#include <iomanip>
#include <iostream>

#include <sys/time.h>
#include "PMMException.hpp"

template <class S>
double	PmergeMe<S>::__GetTimeDiff(timeval & start, timeval & end)
{
	return (end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec - start.tv_usec;
}

template <class S>
bool	PmergeMe<S>::__IsSorted(void) const
{
	const typename S::seq_t seq = __sortable_->GetSequence();
	const typename S::seq_t copy = __sortable_->GetCopy();

	if (__sortable_->GetSequence().size() < 2)
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
		: __sortable_(new S()), __measure_time_(false), __container_name(container_name)
{
	EnableTimeMeasure();
	__container_name = container_name;
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
			double	time_res = __GetTimeDiff(__tv_init_start_, __tv_init_end_);

			std::cout << std::fixed << std::setprecision(3);
			std::cout << "[" << __container_name << "] Initialization: " << time_res << "μs (~" << time_res / 1000L
				<< "ms, ~" << time_res / 1000000L << "s)." << std::endl;
		}
	}
	catch (const std::exception & e)
	{
		delete __sortable_;
		throw;
	}
}

template <class S>
template <class T> PmergeMe<S>::PmergeMe(const PmergeMe<T> & other)
		: __sortable_(new S(other.__sortable_)), __measure_time_(other.__measure_time_), __container_name(other.__container_name) {}

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

		this->__container_name = other.__container_name;
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
		double	time_res = __GetTimeDiff(__tv_sort_start_, __tv_sort_end_);

		std::cout << std::fixed << std::setprecision(3);
		std::cout << "[" << __container_name << "] Sorting: " << time_res << "μs (~" << time_res / 1000L
			<< "ms, ~" << time_res / 1000000L << "s)." << std::endl;
	}
}

template <class S>
void	PmergeMe<S>::Display(void) const
{
	const typename S::seq_t seq = __sortable_->GetSequence();

	std::cout << "Sequence data: " << (__IsSorted() ? GREEN "sorted" : RED "not sorted");
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
PmergeMe<S>::PmergeMe(void)
		: __sortable_(NULL), __measure_time_(false), __container_name("")
{
	throw PMMException("This class cannot be instanciated without a sequence.");
}