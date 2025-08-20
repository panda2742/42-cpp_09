#include "sort/SortableDeque.hpp"
#include <deque>

SortableDeque::SortableDeque(void): ASortable() {}

SortableDeque::SortableDeque(const SortableDeque & other): ASortable(other) {}

SortableDeque::~SortableDeque(void) {}

SortableDeque &	SortableDeque::operator=(const SortableDeque & other)
{
	if (this != &other)
	{
		this->__sequence_ = other.__sequence_;
		this->__threads_depth_ = other.__threads_depth_;
	}

	return *this;
}

SortableDeque::seq_t &	SortableDeque::Sort(void)
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

	pair_seq_t	pairs;

	for (uint64_t	i = 0; i < __sequence_.size(); i += 2)
	{
		if (__sequence_[i] > __sequence_[i + 1])
			pairs.push_front(std::make_pair(__sequence_[i], __sequence_[i + 1]));
		else
			pairs.push_front(std::make_pair(__sequence_[i + 1], __sequence_[i]));
	}

	__sequence_ = __Recursion(pairs, is_odd, isolated_element); 
	return __sequence_;
}

SortableDeque::seq_t	SortableDeque::__Recursion(SortableDeque::pair_seq_t & pairs, bool is_odd, uint64_t isolated_element)
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

	res = __Recursion(primary_pairs, new_is_odd, new_isolated_element);

	__JacobsthalInsert(res, seq_to_insert);

	if (is_odd && isolated_element != ULONG_MAX)
	{
		it_t	pos = res.begin();

		while (pos != res.end() && *pos < isolated_element)
			pos++;

		res.insert(pos, isolated_element);
	}

	return res;
}


void	SortableDeque::__GenerateJacobsthalIndices(SortableDeque::seq_t & indices, uint64_t n) const
{
    static cache_t	cache;

    indices.clear();
    if (n == 0)
        return;

	cache_t::iterator	it = cache.find(n);
    if (it == cache.end())
    {
        seq_t	jacobsthal;
        jacobsthal.push_back(1);
        if (n > 1)
            jacobsthal.push_back(1);

        while (jacobsthal.back() < n)
        {
            uint64_t	next = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
            if (next > n)
                break;
            jacobsthal.push_back(next);
        }

        seq_t		order;
        bool_seq_t	used(n, false);

        order.push_back(0);
        used[0] = true;

        for (uint64_t i = 2; i < jacobsthal.size(); ++i)
        {
            uint64_t	start = jacobsthal[i] - 1;
            uint64_t	end   = jacobsthal[i - 1];

            for (uint64_t j = start; j >= end && j < n; --j)
            {
                if (!used[j])
                {
                    order.push_back(j);
                    used[j] = true;
                }
                if (j == 0)
                    break;
            }
        }

        for (uint64_t i = 0; i < n; ++i)
            if (!used[i])
                order.push_back(i);

        it = cache.insert(std::make_pair(n, std::move(order))).first;
    }

	for (const_it_t jt = it->second.begin(); jt != it->second.end(); ++jt)
	    indices.push_back(static_cast<uint64_t>(*jt));

}

void	SortableDeque::__JacobsthalInsert(seq_t & res, seq_t & seq_to_insert) const
{
	if (seq_to_insert.empty())
		return;

	seq_t	jacobsthal_indices;
	__GenerateJacobsthalIndices(jacobsthal_indices, seq_to_insert.size());

	for (uint64_t	i = 0; i < jacobsthal_indices.size(); i++)
	{
		uint64_t	idx = jacobsthal_indices[i];

		if (idx < seq_to_insert.size())
		{
			uint64_t	element = seq_to_insert[idx];
			const_it_t	pos = __LowerBound(res.begin(), res.end(), element);
			res.insert(pos, element);
		}
	}
}

SortableDeque::const_it_t	SortableDeque::__LowerBound(it_t first, it_t last, uint64_t value) const
{
	it_t		it, temp = first;
	uint64_t	count = 0, step;

	while (temp != last)
	{
		++temp;
		++count;
	}

	while (count > 0)
	{
		it = first;
		step = count / 2;

		for (uint64_t	i = 0; i < step; i++)
			it++;
		
		if (*it < value)
		{
			first = it;
			++first;
			count -= step + 1;
		}
		else count = step;
	}

	return first;
}
