#ifndef DEQUESORTABLE_HPP
#define DEQUESORTABLE_HPP

#include "ASortable.hpp"

#include <inttypes.h>

#include <cstdlib>
#include <deque>

class DequeSortable : public ASortable<sortable_deque_t, deque_pair_t>
{
	public:
		typedef std::deque<bool>	Base;
		typedef sortable_deque_t	ContainerType;

		DequeSortable(void);
		DequeSortable(const DequeSortable & other);
		~DequeSortable(void);

		DequeSortable &	operator=(const DequeSortable & other);

		sortable_deque_t &			Sort(void);

	private:
		virtual sortable_deque_t	__Recursion(deque_pair_t & pairs, bool is_odd, uint64_t isolated_element);
};

#endif
