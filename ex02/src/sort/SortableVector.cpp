#include "sort/SortableVector.hpp"
#include <iostream>

SortableVector::SortableVector(void): ASortable() {}

SortableVector::SortableVector(const SortableVector & other): ASortable(other) {}

SortableVector::~SortableVector(void) {}

SortableVector &	SortableVector::operator=(const SortableVector & other)
{
	if (this != &other)
		this->sequence_ = other.sequence_;

	return *this;
}

SortableVector::seq_t	&	SortableVector::Sort(void)
{
	std::cout << "Sorted list" << std::endl;
	return sequence_;
}

SortableVector::seq_t	SortableVector::Recursion_(SortableVector::pair_seq_t & pairs, bool is_odd, uint64_t isolated_element)
{
	(void)pairs;
	(void)is_odd;
	(void)isolated_element;
	seq_t	null;
	return null;
}

void	SortableVector::JacobsthalInsert_(SortableVector::seq_t & res, SortableVector::seq_t & seq_to_insert)
{
	(void)res;
	(void)seq_to_insert;
}

void	SortableVector::GenerateJacobsthalIndices_(SortableVector::seq_t & indices, uint64_t n)
{
	(void)indices;
	(void)n;
}

SortableVector::it_t	SortableVector::LowerBound_(SortableVector::it_t first, SortableVector::it_t last, uint64_t value)
{
	(void)first;
	(void)last;
	(void)value;
	return sequence_.begin();
}
