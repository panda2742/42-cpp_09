#include "sort/Algorithm.hpp"
#include "sort/SortableDeque.hpp"
#include <deque>
#include <pthread.h>

SortableDeque::SortableDeque(void): ASortable() {}

SortableDeque::SortableDeque(const SortableDeque & other): ASortable(other) {}

SortableDeque::~SortableDeque(void) {}

SortableDeque &	SortableDeque::operator=(const SortableDeque & other)
{
	if (this != &other)
		this->sequence_ = other.sequence_;

	return *this;
}

SortableDeque::seq_t &	SortableDeque::Sort(void)
{
	if (sequence_.size() < 2)
		return sequence_;

	const bool	is_odd = sequence_.size() % 2;
	uint64_t	isolated_element = ULONG_MAX;

	if (is_odd)
	{
		isolated_element = sequence_.back();
		sequence_.pop_back();
	}

	pair_seq_t	pairs;

	for (uint64_t	i = 0; i < sequence_.size(); i += 2)
	{
		if (sequence_[i] > sequence_[i + 1])
			pairs.push_back(std::make_pair(sequence_[i], sequence_[i + 1]));
		else
			pairs.push_back(std::make_pair(sequence_[i + 1], sequence_[i]));
	}

	sequence_ = Recursion_(pairs, is_odd, isolated_element); 
	return sequence_;
}

SortableDeque::seq_t	SortableDeque::Recursion_(SortableDeque::pair_seq_t & pairs, bool is_odd, uint64_t isolated_element)
{
	seq_t	res;

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

	pair_seq_t	primary_pairs;
	seq_t		seq_to_insert;
	bool		new_is_odd = pairs.size() % 2;
	uint64_t	new_isolated_element = ULONG_MAX;

	if (new_is_odd)
	{
		new_isolated_element = pairs.back().first;
		seq_to_insert.push_back(pairs.back().second);
		pairs.pop_back();
	}

	for (uint64_t	i = 0; i < pairs.size(); i += 2)
	{
		if (pairs[i].first > pairs[i + 1].first)
			primary_pairs.push_back(std::make_pair(pairs[i].first, pairs[i + 1].first));
		else
			primary_pairs.push_back(std::make_pair(pairs[i + 1].first, pairs[i].first));

		seq_to_insert.push_back(pairs[i].second);
		seq_to_insert.push_back(pairs[i + 1].second);
	}

	res = Recursion_(primary_pairs, new_is_odd, new_isolated_element);

	JacobsthalInsert_(res, seq_to_insert);

	if (is_odd && isolated_element != ULONG_MAX)
	{
		it_t	pos = res.begin();

		while (pos != res.end() && *pos < isolated_element)
			pos++;

		res.insert(pos, isolated_element);
	}

	return res;
}

void	SortableDeque::JacobsthalInsert_(seq_t & res, seq_t & seq_to_insert)
{
	if (seq_to_insert.empty())
		return;

	seq_t	jacobsthal_indices;
	Algorithm<SortableDeque>::GenerateJacobsthalIndices(jacobsthal_indices, seq_to_insert.size());

	for (uint64_t	i = 0; i < jacobsthal_indices.size(); i++)
	{
		uint64_t	idx = jacobsthal_indices[i];

		if (idx < seq_to_insert.size())
		{
			uint64_t	element = seq_to_insert[idx];
			it_t		pos = Algorithm<SortableDeque>::LowerBound(res.begin(), res.end(), element);
			res.insert(pos, element);
		}
	}
}
