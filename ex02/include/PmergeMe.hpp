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

typedef std::deque<uint64_t>				PMMDeque;
typedef std::vector<uint64_t>				PMMVector;
typedef std::list<uint64_t>					PMMList;
typedef std::pair<uint64_t, uint64_t>		U64Pair;
typedef std::deque<U64Pair>					PMMDequePair;
typedef std::vector<U64Pair>				PMMVectorPair;
typedef std::list<U64Pair>					PMMDListPair;

/**
 * The data used to split the elements and to store the split elements.
 * 
 * @tparam T T is the container that contain the sequence to merge-insert.
 * By default and portability, it can either be a Random Access Iterator
 * container (deque, vector), or it can be a Bidirectionnal Iterator container
 * (list).
 */
template <class T>
struct s_algo_data
{
	/**
	 * The boolean to indicate whether the sequence is odd or not.
	 */
	bool		is_isolated;
	/**
	 * The isolated element of the current sequence.
	 */
	uint64_t	isolated;
	/**
	 * The sequence to apply the merge-insertion on.
	 */
	T			sequence;
};

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
template <class T = PMMDeque, class TPair = PMMDequePair>
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
	 * @tparam Cont Any type of the sequences containers. If it is not, a
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
	 * @tparam Cont Any type of the sequences containers. If it is not, a
	 * compile-time error will occur.
	 * @tparam Cont The same container but for the pairs. It must be the same
	 * type as Cont.
	 */
	template <class Cont, class ContPair>
	PmergeMe<T, TPair> &	operator=(const PmergeMe<Cont, ContPair> & other)
	throw();

	/**
	 * @brief Starts the algorithm and calls the private dispatcher to call the
	 * appropriate signature for the underlying container. Also includes an
	 * internal timer (and returns its result).
	 * 
	 * 1. The algorithm starts with splitting the N values of a sequence into
	 * N/2 pairs (with N the number of elements). If N is odd, the last element
	 * is isolated and not into a pair.
	 * 
	 * 2. The pairs form a set of 2 sequences (one for the ->first elements and
	 * one for the ->second elements), with the first one containing all the
	 * greatest values and the second one containing all the smallest values. We
	 * repeat the step 1 on each sequence (and so on). This algorithm is called
	 * recursively until it is not possible anymore; and this sorts all the
	 * elements and make them ready to be inserted again, it is called the
	 * insertion list.
	 * 
	 * 3. Creation of the Jacobsthal sequence J(n) by going from the greatest
	 * index to the smallest index.
	 * 
	 * 4. The optimal insertion: the first element of the insertion list is
	 * inserted in first place. Then, following the order given by the
	 * Jacobsthal sequence, we do a binary research to find the exact position
	 * into the main list. Once the position is found, we step aside the 
	 * elements if necessary and we insert the element into the correct
	 * position.
	 * 
	 * 5. The insertion of the last element is the last step. If the list was
	 * odd-sized, we have an isolated element. We use a binary research and we
	 * do the step 4 on it.
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
	 * @tparam Container The Random Access Iterator container type.
	 * @tparam PairContainer The Random Access Iterator container type but for
	 * the pairs (same type as Container)
	 */
	template <class Container, class PairContainer>
	void	_SortImpl(
		struct s_algo_data<Container> & data,
		PairContainer,
		std::random_access_iterator_tag
	)
	throw(PMMException);
	/**
	 * The sorting algorithm for the bidirectionnal containers such as list in
	 * this use case. Use this algorithm with the PMMList container.
	 * 
	 * @param o The container to sort.
	 * @param tag A tag representing the iterator category.
	 * @throws Can throw an error if something unexpected occurs.
	 * 
	 * @tparam Container The Bidirectionnal Iterator container type.
	 * @tparam PairContainer The Bidirectionnal Iterator container type but for
	 * the pairs (same type as Container)
	 */
	template <class Container, class PairContainer>
	void	_SortImpl(
		struct s_algo_data<Container> & data,
		PairContainer,
		std::bidirectional_iterator_tag
	)
	throw(PMMException);
};

#include "PmergeMe.tpp"

#endif /* PMERGE_ME_HPP_ */
