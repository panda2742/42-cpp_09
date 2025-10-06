#ifndef LOGIC_ALGO_HPP
#define LOGIC_ALGO_HPP

#include <stdint.h> // For uint32_t
#include <cstdlib> // for size_t
#include <deque> // for deque
#include <vector> // for vector
#include <iterator> // for iterator_traits
#include <map> // for map

namespace algo
{
	namespace utils
	{
		template <class T> void	reserve(T& ctn, size_t new_cap);

		template <class T>
		void	reserve(std::vector<uint32_t>& ctn, size_t new_cap);

		template <class T>
		void	reserve(std::deque<uint32_t>& ctn, size_t new_cap);
	}

	template <class It> It										lower_bound(It first, It last, uint32_t value);
	template <class T, class BoolT> void						generate_jacobsthal(T& indices, uint32_t n);
	template <class T, class It, class BoolT> void				jacobsthal_insert(T& res, T& to_insert);
	template <class T, class PairT, class BoolT> T&				sort(T& sequence);
	template <class T, class PairT, class It, class BoolT> T	recursion(PairT& pairs, bool is_odd, uint32_t isolated_value);
}

#include "algo.tpp"

#endif
