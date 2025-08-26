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
		seq_t	Recursion_(pair_seq_t & pairs, bool is_odd, uint64_t isolated_element);
		void	JacobsthalInsert_(seq_t & res, seq_t & seq_to_insert);
};

#endif