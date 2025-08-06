#include "PmergeMe.hpp"

#include <iostream>

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <ctime>

template <class T, class TPair>
PmergeMe<T, TPair>::PmergeMe(void) throw(): _seq_size(0)
{}

template <class T, class TPair>
PmergeMe<T, TPair>::PmergeMe(const char **seq, size_t seq_size) throw(PMMException)
	: _seq_size(0)
{
	typedef is_allowed_container<T>	_check;
	(void)sizeof(_check);

	Fill(seq, seq_size);
}

template <class T, class TPair>
template <class Cont>
PmergeMe<T, TPair>::PmergeMe(const PmergeMe<Cont> & other) throw(): _seq_size(0)
{
	typedef is_allowed_container<Cont>	_check;
	(void)sizeof(_check());

	this->_data.assign(other._data.begin(), other._data.end());
	this->_seq_size = other._seq_size;
}

template <class T, class TPair>
PmergeMe<T, TPair>::~PmergeMe(void) throw()
{}

template <class T, class TPair>
template <class Cont, class ContPair>
PmergeMe<T, TPair> &	PmergeMe<T, TPair>::operator=(const PmergeMe<Cont, ContPair> & other) throw()
{
	typedef is_allowed_container<Cont>	_check;
	(void)sizeof(_check());

	this->_data.assign(other._data.begin(), other._data.end());
}

template <class T, class TPair>
double	PmergeMe<T, TPair>::Fill(const char **seq, size_t seq_size) throw(PMMException)
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

template <class T, class TPair>
double	PmergeMe<T, TPair>::Sort(void) throw(PMMException)
{
	std::clock_t	start = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to start the clock.");

	typedef struct s_algo_data<T>	SData;
	SData	data_wrapper;
	data_wrapper.sequence = _data;
	_SortImpl(data_wrapper, TPair(), ContainerCategory());

	std::clock_t	end = std::clock();

	if (start == static_cast<std::clock_t>(-1))
		throw PMMException("Error: failed to end the clock.");
	
	double	duration = static_cast<double>(end - start);
	
	return duration / CLOCKS_PER_SEC;
}

template <class T, class TPair>
void	PmergeMe<T, TPair>::Display(void) const throw()
{
	typename T::const_iterator	it = _data.begin();
	typename T::const_iterator	end = _data.end();

	std::cout << GREY "\nElements:\n" << std::endl;
	for (; it != end; ++it)
		std::cout << *it << " ";
	std::cout << RESET "\n" << std::endl;
}

// INFO PUBLIC DISPATCHED SORTING ALGORITHMS:    ///////////////////////////////

// INFO RANDOM ACCESS ITERATOR IMPLEMENTATION

template <class T, class TPair>
template <class Container, class PairContainer>
void	PmergeMe<T, TPair>::_SortImpl(
	struct s_algo_data<Container> & data,
	PairContainer,
	std::random_access_iterator_tag
)
throw(PMMException)
{
	typedef struct s_algo_data<Container>	SData;

	if (data.sequence.size() <= 1)
		return ;

	PairContainer	pairs;

	if (data.sequence.size() % 2)
	{
		data.is_isolated = true;
		data.isolated = data.sequence.back();
		data.sequence.pop_back();
	}

	size_t	seq_len = data.sequence.size();
	for (size_t	i = 0; i < seq_len; i += 2)
	{
		if (data.sequence[i + 1] > data.sequence[i])
			pairs.push_back(
				std::make_pair(data.sequence[i + 1], data.sequence[i])
			);
		else
			pairs.push_back(
				std::make_pair(data.sequence[i], data.sequence[i + 1])
			);
	}

	SData	primary_seq_data, secondary_seq_data;

	seq_len = pairs.size();
	for (size_t	i = 0; i < seq_len; i++)
	{
		primary_seq_data.sequence.push_back(pairs[i].first);
		secondary_seq_data.sequence.push_back(pairs[i].second);
	}

	_SortImpl(primary_seq_data, PairContainer(), std::random_access_iterator_tag());
}

// INFO BIDIRECTIONNAL ITERATOR IMPLEMENTATION

template <class T, class TPair>
template <class Container, class PairContainer>
void	PmergeMe<T, TPair>::_SortImpl(
	struct s_algo_data<Container> & data,
	PairContainer,
	std::bidirectional_iterator_tag
)
throw(PMMException)
{
	(void)data;
}
