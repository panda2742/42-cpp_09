#ifndef DEQUESORTABLE_HPP
#define DEQUESORTABLE_HPP

#include "ASortable.hpp"

#include <inttypes.h>

#include <cstdlib>
#include <deque>

class DequeSortable : public ASortable<sortable_deque_t>
{
	public:
		DequeSortable(void) throw();
		DequeSortable(const DequeSortable & other) throw();
		~DequeSortable(void) throw();

		DequeSortable &	operator=(const DequeSortable & other) throw();

		void	Sort(void) const throw();
};

#endif
