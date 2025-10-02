#include "algo.hpp"

namespace algo
{
	namespace utils
	{
		template <class T> void	reserve(T& ctn, size_t new_cap) { (void)ctn; (void)new_cap; }
		template <class T> void	reserve(std::vector<uint32_t>& ctn, size_t new_cap) { ctn.reserve(new_cap); }
		template <class T> void	reserve(std::deque<uint32_t>& ctn, size_t new_cap) { (void)ctn; (void)new_cap; }
	}
}
