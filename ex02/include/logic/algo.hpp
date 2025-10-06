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

	class Mutex
	{
	public:
		Mutex(void);
		~Mutex(void);
		Mutex(const Mutex&);
		Mutex&	operator=(const Mutex&);
		void	lock(void);
		void	unlock(void);

	private:
		pthread_mutex_t	mutex_;
	};

	class LockGuard
	{
	public:
		LockGuard(void);
		~LockGuard(void);
		LockGuard(const LockGuard&);
		LockGuard&	operator=(const LockGuard&);

	private:
		Mutex&	mutex_;
	};
}

#include "algo.tpp"

#endif
