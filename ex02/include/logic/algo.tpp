#include "algo.hpp"

namespace algo
{
	namespace utils
	{
		template <class T>
		void	reserve(T& ctn, size_t new_cap) { (void)ctn; (void)new_cap; }

		template <class T>
		void	reserve(std::vector<uint32_t>& ctn, size_t new_cap) { ctn.reserve(new_cap); }

		template <class T>
		void	reserve(std::deque<uint32_t>& ctn, size_t new_cap) { (void)ctn; (void)new_cap; }
	}

	template <class It> It	lower_bound(It first, It last, uint32_t value)
	{
		typename std::iterator_traits<It>::difference_type	count = last - first, step;
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

	template <class T, class BoolT> void	generate_jacobsthal(T& indices, uint32_t n)
	{
		static	std::map<uint32_t, T>	cache;

		indices.clear();
		if (n == 0) return;

		typename std::map<uint32_t, T>::iterator	it = cache.find(n);

		if (it == cache.end())
		{
			T	jacobsthal;
			jacobsthal.push_back(1);

			if (n > 1) jacobsthal.push_back(1);

			while (jacobsthal.back() < n)
			{
				uint32_t	next = jacobsthal.back() + 2 * jacobsthal.at(jacobsthal.size() - 2);
				if (next > n) break;
				jacobsthal.push_back(next);
			}

			T		order;
			BoolT	used(n, false);

			order.push_back(0);
			used[0] = true;

			for (uint32_t	i = 2; i < jacobsthal.size(); ++i)
			{
				uint32_t	start = jacobsthal.at(i) - 1;
				uint32_t	end = jacobsthal.at(i - 1);

				for (uint32_t	j = start; j > end && j < n; --j)
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

			for (uint32_t	i = 0; i < n; ++i)
				if (!used[i]) order.push_back(i);

			it = cache.insert(std::make_pair(n, order)).first;
		}

		for (typename T::const_iterator	jt = it->second.begin(); jt != it->second.end(); ++jt)
			indices.push_back(static_cast<uint32_t>(*jt));
	}

	template <class T, class It, class BoolT> void	jacobsthal_insert(T& res, T& to_insert)
	{
		if (to_insert.empty()) return;

		T	jacobsthal_indices;

		generate_jacobsthal<T, BoolT>(jacobsthal_indices, to_insert.size());

		for (uint32_t	i = 0; i < jacobsthal_indices.size(); ++i)
		{
			uint32_t	idx = jacobsthal_indices[i];

			if (idx < to_insert.size())
			{
				uint32_t	element = to_insert[idx];
				It			pos = lower_bound<It>(res.begin(), res.end(), element);
				res.insert(pos, element);
			}
		}
	}

	template <class T, class PairT, class BoolT> T&	sort(T& sequence)
	{
		const bool	is_odd = sequence.size() % 2;
		uint32_t	isolated_element = 0xffffffff;

		if (is_odd)
		{
			isolated_element = sequence.back();
			sequence.pop_back();
		}

		PairT	pairs;

		for (uint32_t	i = 0; i < sequence.size(); i += 2)
		{
			if (sequence[i] > sequence[i + 1])
				pairs.push_back(std::make_pair(sequence[i], sequence[i + 1]));
			else
				pairs.push_back(std::make_pair(sequence[i + 1], sequence[i]));
		}

		sequence = recursion<T, PairT, typename T::iterator, BoolT>(pairs, is_odd, isolated_element);
		return sequence;
	}

	template <class T, class PairT, class It, class BoolT> T	recursion(PairT& pairs, bool is_odd, uint32_t isolated_value)
	{
		T	res;

		if (pairs.empty())
		{
			if (is_odd && isolated_value != 0xffffffff)
				res.push_back(isolated_value);

			return res;
		}

		if (pairs.size() == 1)
		{
			res.push_back(pairs[0].second);
			res.push_back(pairs[0].first);

			if (is_odd && isolated_value != 0xffffffff)
			{
				if (isolated_value < res[0]) res.insert(res.begin(), isolated_value);
				else if (isolated_value < res[1]) res.insert(res.begin() + 1, isolated_value);
				else res.push_back(isolated_value);
			}

			return res;
		}

		PairT		primary_pairs;
		T			seq_to_insert;
		bool		new_is_odd = pairs.size() % 2;
		uint32_t	new_isolated_value = 0xffffffff;

		if (new_is_odd)
		{
			new_isolated_value = pairs.back().first;
			seq_to_insert.push_back(pairs.back().second);
			pairs.pop_back();
		}

		for (uint32_t	i = 0; i < pairs.size(); i += 2)
		{
			if (pairs[i].first > pairs[i + 1].first)
				primary_pairs.push_back(std::make_pair(pairs[i].first, pairs[i + 1].first));
			else
				primary_pairs.push_back(std::make_pair(pairs[i + 1].first, pairs[i].first));

			seq_to_insert.push_back(pairs[i].second);
			seq_to_insert.push_back(pairs[i + 1].second);
		}

		res = recursion<T, PairT, It, BoolT>(primary_pairs, new_is_odd, new_isolated_value);

		jacobsthal_insert<T, It, BoolT>(res, seq_to_insert);

		if (is_odd && isolated_value != 0xffffffff)
		{
			It	pos = res.begin();
			while (pos != res.end() && *pos < isolated_value)
				++pos;
			res.insert(pos, isolated_value);
		}

		return res;
	}
}
