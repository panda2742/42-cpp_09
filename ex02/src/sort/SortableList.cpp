#include "sort/SortableList.hpp"
#include <iostream>

SortableList::SortableList(void): ASortable() {}

SortableList::SortableList(const SortableList & other): ASortable(other) {}

SortableList::~SortableList(void) {}

SortableList &	SortableList::operator=(const SortableList & other)
{
	if (this != &other)
		this->sequence_ = other.sequence_;

	return *this;
}

SortableList::seq_t	&	SortableList::Sort(void)
{
	std::cout << "Sorted list" << std::endl;
	return sequence_;
}

SortableList::seq_t	SortableList::Recursion_(SortableList::pair_seq_t & pairs, bool is_odd, uint64_t isolated_element)
{
	(void)pairs;
	(void)is_odd;
	(void)isolated_element;
	seq_t	null;
	return null;
}

void	SortableList::JacobsthalInsert_(SortableList::seq_t & res, SortableList::seq_t & seq_to_insert)
{
	(void)res;
	(void)seq_to_insert;
}

void	SortableList::GenerateJacobsthalIndices_(SortableList::seq_t & indices, uint64_t n)
{
	(void)indices;
	(void)n;
}

SortableList::it_t	SortableList::LowerBound_(SortableList::it_t first, SortableList::it_t last, uint64_t value)
{
	(void)first;
	(void)last;
	(void)value;
	return sequence_.begin();
}
