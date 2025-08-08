#ifndef LISTSORTABLE_HPP
#define LISTSORTABLE_HPP

#include "ASortable.hpp"

#include <inttypes.h>

#include <cstdlib>
#include <list>

typedef std::list<uint64_t>	sortable_list_t;

class ListSortable : public ASortable<sortable_list_t>
{
	public:
		ListSortable(void) throw();
		ListSortable(const ListSortable & other) throw();
		~ListSortable(void) throw();

		ListSortable &	operator=(const ListSortable & other) throw();

		void	Sort(void) const throw();
};

#endif
