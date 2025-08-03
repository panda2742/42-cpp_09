#include "PmergeMe.hpp"

#include <iostream>

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <ctime>

template <class T>
PmergeMe<T>::PmergeMe(void) throw()
{}

template <class T>
PmergeMe<T>::PmergeMe(const char **seq, size_t seq_size) throw(PMMException)
{
	typedef is_allowed_container<T>	_check;
	(void)sizeof(_check);

	Fill(seq, seq_size);
}

template <class T>
template <class Cont>
PmergeMe<T>::PmergeMe(const PmergeMe<Cont> & other) throw()
{
	typedef is_allowed_container<Cont>	_check;
	(void)sizeof(_check());

	this->_data.assign(other._data.begin(), other._data.end());
}

template <class T>
PmergeMe<T>::~PmergeMe(void) throw()
{}

template <class T>
template <class Cont>
PmergeMe<T> &	PmergeMe<T>::operator=(const PmergeMe<Cont> & other) throw()
{
	typedef is_allowed_container<Cont>	_check;
	(void)sizeof(_check());

	this->_data.assign(other._data.begin(), other._data.end());
}

template <class T>
double	PmergeMe<T>::Fill(const char **seq, size_t seq_size) throw(PMMException)
{
	std::clock_t	start = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to start the clock.");

	size_t	i = 0;

	while (i < seq_size)
	{
		const std::string	str = std::string(seq[i]);

		if (str.empty())
			throw PMMException("A sequence element cannot be empty.");

		errno = 0;
		char		*end_ptr;
		uint64_t	value = strtoull(seq[i], &end_ptr, 10);

		if (errno == ERANGE && (value == ULLONG_MAX || value == 0))
			throw PMMException("A sequence element cannot be out of range.");

		std::cout << static_cast<int>(*end_ptr) << std::endl;
		if (*end_ptr != 0 || !isdigit(seq[i][0]))
			throw PMMException("A sequence element is not a uint64 number.");

		_data.push_back(value);
		i++;
	}

	std::clock_t	end = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to end the clock.");
	
	double	duration = static_cast<double>(end - start);
	
	return duration / CLOCKS_PER_SEC;
}

template <class T>
double	PmergeMe<T>::Sort(void) throw(PMMException)
{
	std::clock_t	start = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to start the clock.");

	_SortImpl(_data, ContainerCategory());

	std::clock_t	end = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to end the clock.");
	
	double	duration = static_cast<double>(end - start);
	
	return duration / CLOCKS_PER_SEC;
}

template <class T>
void	PmergeMe<T>::Display(void) const throw()
{
	typename T::const_iterator	it = _data.begin();
	typename T::const_iterator	end = _data.end();

	std::cout << GREY "\nElements:\n" << std::endl;
	for (; it != end; ++it)
		std::cout << *it << " ";
	std::cout << RESET "\n" << std::endl;
}

// PUBLIC DISPATCHED SORTING ALGORITHMS IMPL

template <class T>
template <class Container>
void	PmergeMe<T>::_SortImpl(Container & o, std::random_access_iterator_tag)
throw(PMMException)
{
	(void)o;
}

template <class T>
template <class Container>
void	PmergeMe<T>::_SortImpl(Container & o, std::bidirectional_iterator_tag)
throw(PMMException)
{
	(void)o;
}
