#ifndef PMERGE_ME_HPP_
#define PMERGE_ME_HPP_

#include <deque>
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
typedef std::list<U64Pair>					PMMListPair;

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

template <class S>
class PmergeMe
{
public:
	PmergeMe(void) throw();
	PmergeMe(const char **seq, size_t seq_size) throw(PMMException);
	template <class T> PmergeMe(const PmergeMe<T> & other) throw();

	~PmergeMe(void) throw();

	template <class T> PmergeMe<S> &	operator=(const PmergeMe<T> & other)
		throw();

	double	FordJohnson(void) throw(PMMException);
	void	Display(void) const throw();
private:
	S	_sortable;
};

#include "PmergeMe.tpp"

#endif /* PMERGE_ME_HPP_ */
