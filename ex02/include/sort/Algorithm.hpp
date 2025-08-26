#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <inttypes.h>
#include <iterator>

#include "sort/Algorithm.hpp"
#include "sort/SortableDeque.hpp"
#include "sort/SortableVector.hpp"
#include "sort/SortableList.hpp"

template <class Sortable>
class Algorithm
{
	public:
		typedef typename Sortable::it_t			It;
		typedef typename Sortable::const_it_t	ConstIt;
		typedef typename Sortable::cache_t		Cache;
		typedef typename Sortable::seq_t		Seq;
		typedef typename Sortable::bool_seq_t	BoolSeq;

		static It	LowerBound(It first, It last, uint64_t value);
		static void	GenerateJacobsthalIndices(Seq & indices, uint64_t n);
	private:
		Algorithm(void);
		~Algorithm(void);
		Algorithm(const Algorithm & other);
		Algorithm &	operator=(const Algorithm & other);

		static It	LowerBoundImpl_(It first, It last, uint64_t value, std::random_access_iterator_tag);
		static It	LowerBoundImpl_(It first, It last, uint64_t value, std::bidirectional_iterator_tag);
		static void	GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::random_access_iterator_tag);
		static void	GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::bidirectional_iterator_tag);
};

#include "Algorithm.tpp"

#endif