#ifndef VECTORSORTABLE_HPP
#define VECTORSORTABLE_HPP

#include "ASortable.hpp"

#include <inttypes.h>

#include <cstdlib>
#include <vector>

typedef std::vector<uint64_t>	sortable_vector_t;
typedef std::vector< std::pair<uint64_t, uint64_t> >	vector_pair_t;

class VectorSortable : public ASortable<sortable_vector_t, vector_pair_t>
{
	public:
		typedef std::vector<bool>	Base;
		typedef sortable_vector_t	ContainerType;

		VectorSortable(void);
		VectorSortable(const VectorSortable & other);
		~VectorSortable(void);

		VectorSortable &	operator=(const VectorSortable & other);

		sortable_vector_t &			Sort(void);

	private:
		virtual sortable_vector_t	__Recursion(vector_pair_t & pairs, bool is_odd, uint64_t isolated_element);
};

#endif
