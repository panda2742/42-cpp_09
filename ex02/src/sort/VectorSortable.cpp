#include "sort/VectorSortable.hpp"

#include <iostream>

VectorSortable::VectorSortable(void): ASortable() {}

VectorSortable::VectorSortable(const VectorSortable & other): ASortable(other) {}

VectorSortable::~VectorSortable(void)
{
	__sequence_.clear();
}

VectorSortable &	VectorSortable::operator=(const VectorSortable & other)
{
	if (this != &other)
		this->__sequence_ = other.__sequence_;

	return *this;
}

sortable_vector_t &	VectorSortable::Sort(void)
{
	std::cout << "Sorting vector." << std::endl;

	vector_pair_t	pairs = vector_pair_t();

	__sequence_ = __Recursion(pairs, false, ULONG_MAX);
	return __sequence_;
}

sortable_vector_t	VectorSortable::__Recursion(vector_pair_t & pairs, bool is_odd, uint64_t isolated_element)
{
	(void)pairs;
	(void)is_odd;
	(void)isolated_element;

	return __sequence_;
}
