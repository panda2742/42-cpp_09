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
		seq_t	Recursion_(pair_seq_t & pairs, bool is_odd, uint64_t isolated_element);
		void	JacobsthalInsert_(seq_t & res, seq_t & seq_to_insert);
		void	GenerateJacobsthalIndices_(seq_t & indices, uint64_t n);
		it_t	LowerBound_(it_t first, it_t last, uint64_t value);
};

#endif