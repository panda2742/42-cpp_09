#ifndef SORTABLEVECTOR_HPP
#define SORTABLEVECTOR_HPP

#include <vector>
#include "ASortable.hpp"

class SortableVector : public ASortable<std::vector>
{
	public:
		SortableVector(void);
		SortableVector(const SortableVector & other);
		~SortableVector(void);
		SortableVector &	operator=(const SortableVector & other);

		seq_t &	Sort(void);

	private:
		seq_t	Recursion_(pair_seq_t & pairs, bool is_odd, uint64_t isolated_element);
		void	JacobsthalInsert_(seq_t & res, seq_t & seq_to_insert);
		void	GenerateJacobsthalIndices_(seq_t & indices, uint64_t n);
		it_t	LowerBound_(it_t first, it_t last, uint64_t value);
};

#endif