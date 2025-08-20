#include "sort/SortableVector.hpp"
#include <iostream>

SortableVector::SortableVector(void): ASortable() {}

SortableVector::SortableVector(const SortableVector & other): ASortable(other) {}

SortableVector::~SortableVector(void) {}

SortableVector &	SortableVector::operator=(const SortableVector & other)
{
	if (this != &other)
	{
		this->__sequence_ = other.__sequence_;
		this->__threads_depth_ = other.__threads_depth_;
	}

	return *this;
}

SortableVector::seq_t	&	SortableVector::Sort(void)
{
	std::cout << "Sorted list" << std::endl;
	return __sequence_;
}

SortableVector::seq_t	SortableVector::__Recursion(SortableVector::pair_seq_t & pairs, bool is_odd, uint64_t isolated_element)
{
	(void)pairs;
	(void)is_odd;
	(void)isolated_element;
	seq_t	null;
	return null;
}

void	SortableVector::__JacobsthalInsert(SortableVector::seq_t & res, SortableVector::seq_t & seq_to_insert)
{
	(void)res;
	(void)seq_to_insert;
}

void	SortableVector::__GenerateJacobsthalIndices(SortableVector::seq_t & indices, uint64_t n)
{
	(void)indices;
	(void)n;
}

SortableVector::it_t	SortableVector::__LowerBound(SortableVector::it_t first, SortableVector::it_t last, uint64_t value)
{
	(void)first;
	(void)last;
	(void)value;
	return __sequence_.begin();
}
