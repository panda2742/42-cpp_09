#ifndef VECTORSORTABLE_HPP
#define VECTORSORTABLE_HPP

#include "ASortable.hpp"

#include <inttypes.h>

#include <cstdlib>
#include <vector>

typedef std::vector<uint64_t>	sortable_vector_t;

class VectorSortable : public ASortable<sortable_vector_t>
{
	public:
		VectorSortable(void) throw();
		VectorSortable(const VectorSortable & other) throw();
		~VectorSortable(void) throw();

		VectorSortable &	operator=(const VectorSortable & other) throw();

		void	Sort(void) const throw();
};

#endif
