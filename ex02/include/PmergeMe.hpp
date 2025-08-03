#ifndef PMERGE_ME_HPP_
#define PMERGE_ME_HPP_

#include <deque>
#include <iterator>
#include <list>
#include <string>
#include <vector>

#include <inttypes.h>

#include "PMMException.hpp"

#define GREY "\e[38;2;100;100;100m"
#define RESET "\e[0m"

typedef std::deque<uint64_t>			PMMDeque;
typedef std::vector<uint64_t>			PMMVector;
typedef std::list<uint64_t>				PMMList;
typedef std::pair<uint64_t, uint64_t>	SortPair;

/**
 * Per default, other containers than PMMDeque, PMMD and PMMVector are
 * prohibited from using the copy constructor and copy assignment operator on
 * PmergeMe.
 */
template <class U>
struct	is_allowed_container
{
	static const bool	value = false;
};
template <>
struct	is_allowed_container<PMMDeque>
{
	static const bool	value = true;
};
template <>
struct	is_allowed_container<PMMVector>
{
	static const bool	value = true;
};
template <>
struct	is_allowed_container<PMMList>
{
	static const bool	value = true;
};

/**
 * Represent the PmergeMe class. The class is generic and can hold an underlying
 * container for a better dynamic. Therefore, each container has its algorithm.
 * 
 * @tparam T The T type/class represent a container of the STL C++98. The 
 * container provides a `set' of uint64 elements. Per default, the well
 * managed containers are deque, list and vector.
 */
template <class T = PMMDeque>
class PmergeMe
{
public:
	typedef
		typename std::iterator_traits<typename T::iterator>::iterator_category
		ContainerCategory;

	PmergeMe(void) throw();
	/**
	 * The string array constructor. Takes a double-pointer char towards
	 * (normally) to argv. It reads the sequence and fill the appropriate
	 * container.
	 * 
	 * @param seq The sequence to parse and to store into the underlying 
	 * container.
	 * @param seq_size The size of the sequence to parse.
	 * @throws Can throw an exception if the sequence is invalid.
	 */
	PmergeMe(const char **seq, size_t seq_size) throw(PMMException);
	/**
	 * The copy constructor. Takes another instance of PmergeMe<T>.
	 * 
	 * @param other The other instance of PmergeMe.
	 * 
	 * @tparam Any type of the sequences containers. If it is not, a
	 * compile-time error will occur.
	 */
	template <class Cont>
	PmergeMe(const PmergeMe<Cont> & other) throw();

	~PmergeMe(void) throw();

	/**
	 * The copy assignment operator overload.
	 * 
	 * @param other The other instance of PmergeMe.
	 * 
	 * @tparam Any type of the sequences containers. If it is not, a
	 * compile-time error will occur.
	 */
	template <class Cont>
	PmergeMe<T> &	operator=(const PmergeMe<Cont> & other) throw();

	/**
	 * Starts the algorithm and calls the private dispatcher to call the
	 * appropriate signature for the underlying container. Also includes an
	 * internal timer (and returns its result).
	 * 
	 * @returns The time spent on the algorithm.
	 * @throws Can throw an error if something unexpected occurs.
	 */
	double	Sort(void) throw(PMMException);
	/**
	 * Fill the content of _data with the appropriate method for the underlying
	 * container. Throws an error if the type of the element is not an uint64.
	 * 
	 * @param seq The sequence of C string to parse.
	 * @param seq_size The size of the sequence to parse.
	 * @returns The time spent on the initialization.
	 * @throws Throws an error if something bad is caught with an element of the
	 * sequence or the sequence itself.
	 */
	double	Fill(const char **seq, size_t seq_size) throw(PMMException);
	/**
	 * Display the current elements of the underlying container.
	 */
	void	Display(void) const throw();
private:
	/**
	 * The underlying container of the class. Includes the sequence.
	 */
	T			_data;
	/**
	 * The size of the sequence.
	 */
	size_t		_seq_size;
	/**
	 * The sorting algorithm for the random access iterators such as vector or
	 * deque in this use case. Use this algorithm with the PMMDeque or PMMVector
	 * containers.
	 * 
	 * @param o The container to sort.
	 * @param tag A tag representing the iterator category.
	 * @throws Can throw an error if something unexpected occurs.
	 * 
	 * @tparam The Random Access Iterator container type.
	 */
	template <class Container>
	void	_SortImpl(Container & o, std::random_access_iterator_tag)
	throw(PMMException);
	/**
	 * The sorting algorithm for the bidirectionnal containers such as list in
	 * this use case. Use this algorithm with the PMMList container.
	 * 
	 * @param o The container to sort.
	 * @param tag A tag representing the iterator category.
	 * @throws Can throw an error if something unexpected occurs.
	 * 
	 * @tparam The Bidirectionnal Iterator container type.
	 */
	template <class Container>
	void	_SortImpl(Container & o, std::bidirectional_iterator_tag)
	throw(PMMException);
	/**
	 * This is the first step of the algorithm. We split the values into N/2
	 * pairs with N the number of elements. If the N is odd, the last element is
	 * isolated and not into a pair.
	 */
	void	_PairElements(uint64_t *isolated, bool *is_odd) throw(PMMException);
};

#include "PmergeMe.tpp"

#endif /* PMERGE_ME_HPP_ */
