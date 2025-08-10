#ifndef SORTABLEDEQUE_HPP
#define SORTABLEDEQUE_HPP

#include <deque>
#include "ASortable.hpp"

class SortableDeque : public ASortable<std::deque>
{
	public:
		SortableDeque(void);
		SortableDeque(const SortableDeque & other);
		~SortableDeque(void);
		SortableDeque &	operator=(const SortableDeque & other);

		seq_t &	Sort(void);

	private:
		seq_t		__Recursion(pair_seq_t & pairs, bool is_odd, uint64_t isolated_element);
		void		__JacobsthalInsert(seq_t & res, seq_t & seq_to_insert) const;
		void		__GenerateJacobsthalIndices(seq_t & indices, uint64_t n) const;
		const_it_t	__LowerBound(it_t first, it_t last, uint64_t value) const;
};

#endif