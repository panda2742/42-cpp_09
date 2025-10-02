#ifndef LOGIC_ALGO_HPP
#define LOGIC_ALGO_HPP

#include <stdint.h> // For uint32_t
#include <cstdlib> // for size_t
#include <deque> // for deque
#include <vector> // for vector

namespace algo
{
	namespace utils
	{
		template <class T> void	reserve(T& ctn, size_t new_cap);
		template <class T> void	reserve(std::vector<uint32_t>& ctn, size_t new_cap);
		template <class T> void	reserve(std::deque<uint32_t>& ctn, size_t new_cap);
	}
}

#include "algo.tpp"

#endif
