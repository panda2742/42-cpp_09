#include "sort/ListSortable.hpp"

#include <iostream>

ListSortable::ListSortable(void): ASortable() {}

ListSortable::ListSortable(const ListSortable & other): ASortable(other) {}

ListSortable::~ListSortable(void)
{
	__sequence_.clear();
}

ListSortable &	ListSortable::operator=(const ListSortable & other)
{
	if (this != &other)
		this->__sequence_ = other.__sequence_;

	return *this;
}

sortable_list_t &	ListSortable::Sort(void)
{
	std::cout << "Sorting list." << std::endl;

	list_pair_t	pairs = list_pair_t();

	__sequence_ = __Recursion(pairs, false, ULONG_MAX);
	return __sequence_;
}

sortable_list_t	ListSortable::__Recursion(list_pair_t & pairs, bool is_odd, uint64_t isolated_element)
{
	(void)pairs;
	(void)is_odd;
	(void)isolated_element;

	return __sequence_;
}
