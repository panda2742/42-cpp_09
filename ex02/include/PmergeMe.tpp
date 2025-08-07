#include "PmergeMe.hpp"

#include <iostream>

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <ctime>

template <class S> PmergeMe<S>::PmergeMe(void) throw(): _seq_size(0) {}

template <class S> PmergeMe<S>::PmergeMe(const char **seq, size_t seq_size)
	throw(PMMException)
	: _seq_size(0)
{
	typedef is_allowed_container<T>	_check;
	(void)sizeof(_check);

	Fill(seq, seq_size);
}

template <class S>
template <class T>
PmergeMe<S>::PmergeMe(const PmergeMe<T> & other) throw(): _seq_size(0)
{
	typedef is_allowed_container<Cont>	_check;
	(void)sizeof(_check());

	this->_data.assign(other._data.begin(), other._data.end());
	this->_seq_size = other._seq_size;
}

template <class S> PmergeMe<S>::~PmergeMe(void) throw()
{}

template <class S>
template <class T>
PmergeMe<S> &	PmergeMe<S>::operator=(const PmergeMe<T> & other) throw()
{
	typedef is_allowed_container<Cont>	_check;
	(void)sizeof(_check());

	this->_data.assign(other._data.begin(), other._data.end());
}

template <class S>
double	PmergeMe<S>::Fill(const char **seq, size_t seq_size) throw(PMMException)
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

		if (*end_ptr != 0 || !isdigit(seq[i][0]))
			throw PMMException("A sequence element is not a uint64 number.");

		_data.push_back(value);
		i++;
	}

	_seq_size = seq_size;

	std::clock_t	end = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to end the clock.");
	
	double	duration = static_cast<double>(end - start);
	
	return duration / CLOCKS_PER_SEC;
}

template <class S>
double	PmergeMe<S>::FordJohnson(void) throw(PMMException)
{
	std::clock_t	start = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to start the clock.");

	

	std::clock_t	end = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to end the clock.");
	
	double	duration = static_cast<double>(end - start);
	
	return duration / CLOCKS_PER_SEC;
}

template <class S>
void	PmergeMe<S>::Display(void) const throw()
{
	typename S::const_iterator	it = _data.begin();
	typename S::const_iterator	end = _data.end();

	std::cout << GREY "\nElements:\n" << std::endl;
	for (; it != end; ++it)
		std::cout << *it << " ";
	std::cout << RESET "\n" << std::endl;
}
