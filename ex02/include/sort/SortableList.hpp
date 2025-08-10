#ifndef SORTABLELIST_HPP
#define SORTABLELIST_HPP

#include <list>
#include "ASortable.hpp"

class SortableList : public ASortable<std::list>
{
	public:
		SortableList(void);
		SortableList(const SortableList & other);
		~SortableList(void);
		SortableList &	operator=(const SortableList & other);

		seq_t &	Sort(void);
	
	private:
		seq_t		__Recursion(pair_seq_t & pairs, bool is_odd, uint64_t isolated_element);
		void		__JacobsthalInsert(seq_t & res, seq_t & seq_to_insert) const;
		void		__GenerateJacobsthalIndices(seq_t & indices, uint64_t n) const;
		const_it_t	__LowerBound(it_t first, it_t last, uint64_t value) const;
};

#endif