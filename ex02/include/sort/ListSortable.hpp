#ifndef LISTSORTABLE_HPP
#define LISTSORTABLE_HPP

#include "ASortable.hpp"

#include <inttypes.h>

#include <cstdlib>
#include <list>

typedef std::list<uint64_t>	sortable_list_t;
typedef std::list< std::pair<uint64_t, uint64_t> >	list_pair_t;

class ListSortable : public ASortable<sortable_list_t, list_pair_t>
{
	public:
		typedef std::list<bool>	Base;
		typedef sortable_list_t	ContainerType;
	
		ListSortable(void);
		ListSortable(const ListSortable & other);
		~ListSortable(void);

		ListSortable &	operator=(const ListSortable & other);

		sortable_list_t &		Sort(void);

	private:
		virtual sortable_list_t	__Recursion(list_pair_t & pairs, bool is_odd, uint64_t isolated_element);
};

#endif
