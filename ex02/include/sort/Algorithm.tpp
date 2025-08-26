#include "Algorithm.hpp"

template <class Sortable>
Algorithm<Sortable>::Algorithm(void) {}

template <class Sortable>
Algorithm<Sortable>::~Algorithm(void) {}

template <class Sortable>
Algorithm<Sortable>::Algorithm(const Algorithm & other)
{
	(void)other;
}

template <class Sortable>
Algorithm<Sortable> &	Algorithm<Sortable>::operator=(const Algorithm<Sortable> & other)
{
	(void)other;
	return *this;
}

template <class Sortable>
typename Algorithm<Sortable>::It	Algorithm<Sortable>::LowerBoundImpl_(It first, It last, uint64_t value, std::random_access_iterator_tag)
{
	typename std::iterator_traits<It>::difference_type	count = last - first,
														step;
	It	it;

	while (count > 0)
	{
		step = count / 2;
		it = first + step;

		if (*it < value)
		{
			first = ++it;
			count -= step + 1;
		}
		else count = step;
	}

	return first;
}

template <class Sortable>
typename Algorithm<Sortable>::It	Algorithm<Sortable>::LowerBoundImpl_(It first, It last, uint64_t value, std::bidirectional_iterator_tag)
{
	typename std::iterator_traits<It>::difference_type	count = 0;
	typename std::iterator_traits<It>::difference_type	step;

	It	temp = first;
	while (temp != last)
	{
		++temp;
		++count;
	}

	while (count > 0)
	{
		It	it = first;
		step = count / 2;

		for (typename std::iterator_traits<It>::difference_type	i = 0; i < step; i++)
			++it;

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

template <class Sortable>
typename Algorithm<Sortable>::It	Algorithm<Sortable>::LowerBound(It first, It last, uint64_t value)
{
	return LowerBoundImpl_(first, last, value, typename std::iterator_traits<It>::iterator_category());
}

template <class Sortable>
void	Algorithm<Sortable>::GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::random_access_iterator_tag)
{
	static Cache	cache;

	indices.clear();
	if (n == 0) return;

	typename Cache::iterator	it = cache.find(n);

	if (it == cache.end())
	{
		Seq	jacobsthal;
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

		Seq		order;
		BoolSeq	used(n, false);

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

		it = cache.insert(std::make_pair(n, order)).first;
	}

	for (ConstIt jt = it->second.begin(); jt != it->second.end(); ++jt)
		indices.push_back(static_cast<uint64_t>(*jt));
}

template <class Sortable>
void	Algorithm<Sortable>::GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::bidirectional_iterator_tag)
{
	static Cache	cache;

	indices.clear();
	if (n == 0) return;

	typename Cache::iterator	it = cache.find(n);

	if (it == cache.end())
	{
		Seq	jacobsthal;
		jacobsthal.push_back(1);
		if (n > 1)
			jacobsthal.push_back(1);

		while (jacobsthal.back() < n)
		{
			uint64_t	last = jacobsthal.back(),
						second_last = *(++jacobsthal.rbegin()),
						next = last + 2 * second_last;

			if (next > n)
				break;
			jacobsthal.push_back(next);
		}

		Seq		order;
		BoolSeq	used(n, false);

		typename BoolSeq::iterator	used_it = used.begin();
		*used_it = true;
		order.push_back(0);

		typename Seq::iterator	jacob_it = jacobsthal.begin(),
								jacob_prev_it = jacobsthal.begin();
		++jacob_it;

		for (uint64_t i = 2; i < jacobsthal.size(); ++i)
		{
			++jacob_it;
			++jacob_prev_it;

			uint64_t	start = *jacob_it - 1,
						end = *jacob_prev_it;
			
			for (uint64_t	j = start; j >= end && j < n; --j)
			{
				typename BoolSeq::iterator	current_used_it = used.begin();
				for (uint64_t	k = 0; k < j; ++k)
				{
					if (current_used_it == used.end()) break;
					++current_used_it;
				}

				if (current_used_it != used.end() && !*current_used_it)
				{
					order.push_back(j);
					*current_used_it = true;
				}
				if (j == 0)
					break;
			}
		}

		for (uint64_t i = 0; i < n; ++i)
		{
			typename BoolSeq::iterator	current_used_it = used.begin();
			for (uint64_t	k = 0; k < i; ++k)
			{
				if (current_used_it == used.end()) break;
				++current_used_it;
			}

			if (current_used_it != used.end() && !*current_used_it)
				order.push_back(i);
		}

		it = cache.insert(std::make_pair(n, order)).first;
	}

	for (ConstIt jt = it->second.begin(); jt != it->second.end(); ++jt)
		indices.push_back(static_cast<uint64_t>(*jt));
}

template <class Sortable>
void	Algorithm<Sortable>::GenerateJacobsthalIndices(Seq & indices, uint64_t n)
{
	return GenerateJacobsthalIndicesImpl_(indices, n, typename std::iterator_traits<It>::iterator_category());
}

template <class Sortable>
void
