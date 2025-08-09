#include "sort/DequeSortable.hpp"

#include <iostream>

DequeSortable::DequeSortable(void): ASortable() {}

DequeSortable::DequeSortable(const DequeSortable & other): ASortable(other) {}

DequeSortable::~DequeSortable(void)
{
	__sequence_.clear();
}

DequeSortable &	DequeSortable::operator=(const DequeSortable & other)
{
	if (this != &other)
		this->__sequence_ = other.__sequence_;

	return *this;
}

sortable_deque_t &	DequeSortable::Sort(void)
{
	if (__sequence_.size() < 2)
		return __sequence_;

	const bool	is_odd = __sequence_.size() % 2;
	uint64_t	isolated_element = ULONG_MAX;

	if (is_odd)
	{
		isolated_element = __sequence_.back();
		__sequence_.pop_back();
	}

	deque_pair_t	pairs;

	for (size_t	i = 0; i < __sequence_.size(); i += 2)
	{
		if (__sequence_[i] > __sequence_[i + 1])
			pairs.push_front(std::make_pair(__sequence_[i], __sequence_[i + 1]));
		else
			pairs.push_front(std::make_pair(__sequence_[i + 1], __sequence_[i]));
	}

	__sequence_ = __Recursion(pairs, is_odd, isolated_element); 
	return __sequence_;
}

sortable_deque_t	DequeSortable::__Recursion(deque_pair_t & pairs, bool is_odd, uint64_t isolated_element)
{
	sortable_deque_t	res;

	if (pairs.empty())
	{
		if (is_odd && isolated_element != ULONG_MAX)
			res.push_back(isolated_element);

		return res;
	}

	if (pairs.size() == 1)
	{
		res.push_back(pairs[0].second);
		res.push_back(pairs[0].first);

		if (is_odd && isolated_element != ULONG_MAX)
		{
			if (isolated_element < res[0]) res.push_front(isolated_element);
			else if (isolated_element < res[1]) res.insert(res.begin() + 1, isolated_element);
			else res.push_back(isolated_element);
		}

		return res;
	}

	deque_pair_t		primary_pairs;
	sortable_deque_t	seq_to_insert;
	bool				new_is_odd = pairs.size() % 2;
	uint64_t			new_isolated_element = ULONG_MAX;

	if (new_is_odd)
	{
		new_isolated_element = pairs.back().first;
		seq_to_insert.push_back(pairs.back().second);
		pairs.pop_back();
	}

	for (size_t	i = 0; i < pairs.size(); i += 2)
	{
		if (pairs[i].first > pairs[i + 1].first)
			primary_pairs.push_back(std::make_pair(pairs[i].first, pairs[i + 1].first));
		else
			primary_pairs.push_back(std::make_pair(pairs[i + 1].first, pairs[i].first));
		
		seq_to_insert.push_back(pairs[i].second);
		seq_to_insert.push_back(pairs[i + 1].second);
	}

	res = __Recursion(primary_pairs, new_is_odd, new_isolated_element);

	__JacobsthalInsert(res, seq_to_insert);

	if (is_odd && isolated_element != ULONG_MAX)
	{
		sortable_deque_t::iterator	pos = res.begin();

		while (pos != res.end() && *pos < isolated_element)
			pos++;
		
		res.insert(pos, isolated_element);
	}

	return res;
}
