#include "PmergeMe.hpp"

#include <iostream>

#include "PMMException.hpp"

template <class S>
PmergeMe<S>::PmergeMe(const char **seq, size_t seq_size) throw(typename S::SortableInvalidElement)
		: _sortable(new S()), _measure_time(false)
{
	try
	{
		_sortable->Fill(seq, seq_size);
	}
	catch (const std::exception & e)
	{
		throw;
	}
}

template <class S>
template <class T> PmergeMe<S>::PmergeMe(const PmergeMe<T> & other) throw()
		: _sortable(new S(other._sortable)), _measure_time(other._measure_time) {}

template <class S>
PmergeMe<S>::~PmergeMe(void) throw()
{
	if (_sortable)
		delete _sortable;
}

template <class S>
PmergeMe<S> &	PmergeMe<S>::operator=(const PmergeMe<S> & other) throw()
{
	if (this != *other)
	{
		if (this->_sortable)
			delete this->_sortable;

		this->_sortable = new S(other._sortable);
		this->_measure_time = other._measure_time;
	}

	return *this;
}

template <class S>
void	PmergeMe<S>::FordJohnson(void) throw()
{
	_sortable->Sort();
}

template <class S>
void	PmergeMe<S>::Display(void) const throw()
{
	std::cout << "TODO" << std::endl;
}

template <class S>
void	PmergeMe<S>::EnableTimeMeasure(void) throw()
{
	_measure_time = true;
}

template <class S>
void	PmergeMe<S>::DisableTimeMeasure(void) throw()
{
	_measure_time = false;
}

template <class S>
bool	PmergeMe<S>::IsTimeMeasureEnabled(void) const throw()
{
	return _measure_time;
}

template <class S>
S	*PmergeMe<S>::GetSortable(void) const throw()
{
	return _sortable;
}

template <class S>
PmergeMe<S>::PmergeMe(void) throw(PMMException)
		: _sortable(NULL), _measure_time(false)
{
	throw PMMException("This class cannot be instanciated without a sequence.");
}