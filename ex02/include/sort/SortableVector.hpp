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
		seq_t		__Recursion(pair_seq_t & pairs, bool is_odd, uint64_t isolated_element);
		void		__JacobsthalInsert(seq_t & res, seq_t & seq_to_insert) const;
		void		__GenerateJacobsthalIndices(seq_t & indices, uint64_t n) const;
		const_it_t	__LowerBound(it_t first, it_t last, uint64_t value) const;
};

#endif