#include "sort/SortableList.hpp"
#include <iostream>

SortableList::SortableList(void): ASortable() {}

SortableList::SortableList(const SortableList & other): ASortable(other) {}

SortableList::~SortableList(void) {}

SortableList &	SortableList::operator=(const SortableList & other)
{
	if (this != &other)
	{
		this->__sequence_ = other.__sequence_;
		this->__threads_depth_ = other.__threads_depth_;
	}

	return *this;
}

SortableList::seq_t	&	SortableList::Sort(void)
{
	std::cout << "Sorted list" << std::endl;
	return __sequence_;
}

SortableList::seq_t	SortableList::__Recursion(SortableList::pair_seq_t & pairs, bool is_odd, uint64_t isolated_element)
{
	(void)pairs;
	(void)is_odd;
	(void)isolated_element;
	seq_t	null;
	return null;
}

void	SortableList::__JacobsthalInsert(SortableList::seq_t & res, SortableList::seq_t & seq_to_insert) const
{
	(void)res;
	(void)seq_to_insert;
}

void	SortableList::__GenerateJacobsthalIndices(SortableList::seq_t & indices, uint64_t n) const
{
	(void)indices;
	(void)n;
}

SortableList::const_it_t	SortableList::__LowerBound(SortableList::it_t first, SortableList::it_t last, uint64_t value) const
{
	(void)first;
	(void)last;
	(void)value;
	return __sequence_.begin();
}
