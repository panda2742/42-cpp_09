#include "PmergeMe.hpp"
#include <sys/time.h>
#include "utils.hpp"
#include <iostream>
#include <stdint.h>
#include <cstring>
#include <set>
#include <pthread.h>
#include "logic/threads.hpp"
#include "logic/algo.hpp"

// ===========================================================
// |                     PRIVATE MEMBERS                     |
// ===========================================================

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::ford_johnson_(void *)
{
	std::cout << "container:" << container_name_ << "|threads_sort:" << threads::nb_sort_threads << std::endl;

	if (sequence_.size() == 1)
		return;
}

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::display_(void) const
{
	std::cout << "container:" << container_name_ << "|seq_state:" << (is_sorted_() ? "sorted" : "not sorted");
	std::cout << "|seq:";
	for (const_it_t	it = sequence_.begin(); it != sequence_.end(); it++)
	{
		if (it != sequence_.begin())
			std::cout << ";";
		std::cout << *it;
	}
	std::cout << std::endl;
}

template <template <class T, class Alloc> class Ctn>
template <class Class>
void	PmergeMe<Ctn>::chrono_(const std::string& message, void (Class::*f)(void *), void *f_args)
{
	struct timeval	start = { .tv_sec = 0, .tv_usec = 0 },
					end = { .tv_sec = 0, .tv_usec = 0 };

	gettimeofday(&start, NULL);
	Class	*obj = static_cast<Class *>(this);
	(obj->*f)(f_args);
	gettimeofday(&end, NULL);

	uint32_t	diff = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
	std::cout << "container:" << container_name_ << "|" << message << ":" << diff << std::endl;
}

template <template <class T, class Alloc> class Ctn>
bool	PmergeMe<Ctn>::is_sorted_(void) const
{
	if (copy_.size() != sequence_.size()) return false;
	if (sequence_.size() < 2) return true;

	const_it_t	it = sequence_.begin();
	const_it_t	next = it;
	++next;

	for (; next != sequence_.end(); ++it, ++next)
		if (*it > *next) return false;
	return true;
}

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::fill_(void *args)
{
	const char	**seq = reinterpret_cast<fill_args_t *>(args)->seq;
	uint32_t	seq_size = reinterpret_cast<fill_args_t *>(args)->seq_size;

	if (seq_size == 0)
	{
		sequence_.clear();
		return;
	}

	if (seq_size < THRESHOLD_THREADED_FILL)
	{
		std::cout << "container:" << container_name_ << "|threads_init:0" << std::endl;
		std::set<uint32_t>	seen;
		for (uint32_t	i = 0; i < seq_size; i++)
		{
			char		*endptr;
			if (strlen(seq[i]) == 0 || seq[i][0] == '-') throw std::runtime_error("Invalid sequence elemnt format.");
			errno = 0;
			uint32_t	value = std::strtoul(seq[i], &endptr, 10);
			if (
				*endptr != 0 ||
				(value == 0xffffffff && errno == ERANGE) ||
				!seen.insert(value).second
			) throw std::runtime_error("Invalid sequence element format.");
			sequence_.push_back(value);
		}
		copy_ = sequence_;
		return;
	}

	unsigned short int	max_threads = get_hardware_concurrency();
	if (max_threads == 0) max_threads = 2;

	uint32_t	potential = seq_size / THREADED_FILL_CHUNK_SIZE;
	if (potential == 0) potential = 1;

	unsigned short int	nthreads = static_cast<unsigned short int>(potential);
	if (nthreads > max_threads) nthreads = max_threads;
	else if (nthreads > seq_size) nthreads = static_cast<unsigned short int>(seq_size);

	uint32_t	block = (seq_size + nthreads - 1) / nthreads;

	pthread_t						*threads = new pthread_t[nthreads];
	threads::fillchunk_args<seq_t>	*t_args = new threads::fillchunk_args<seq_t>[nthreads];
	seq_t							*locals = new seq_t[nthreads];

	std::cout << "container:" << container_name_ << "|threads_init:" << nthreads << std::endl;

	unsigned int	t = 0;
	for (; t < nthreads; t++)
	{
		uint32_t	start = static_cast<uint32_t>(t) * block,
					end = start + block;
		if (start >= seq_size) start = end = seq_size;
		if (end > seq_size) end = seq_size;

		t_args[t].seq		= seq;
		t_args[t].start	= start;
		t_args[t].end		= end;
		t_args[t].out		= &locals[t];
		t_args[t].error	= false;
		pthread_create(&threads[t], NULL, threads::fill_chunk_job<seq_t>, &t_args[t]);
	}

	bool	error = false;
	for (t = 0; t < nthreads; t++)
	{
		pthread_join(threads[t], NULL);
		if (t_args[t].error) error = true;
	}
	if (error)
	{
		delete[] threads; delete[] t_args; delete[] locals;
		throw std::runtime_error("Invalid sequence elemnt format on a thread.");
	}

	sequence_.clear();
	std::set<uint32_t>	seen;
	for (t = 0; t < nthreads; t++)
	{
		for (const_it_t	it = locals[t].begin(); it != locals[t].end(); it++)
		{
			if (!seen.insert(*it).second)
			{
				delete[] threads; delete[] t_args; delete[] locals;
				throw std::runtime_error("There is a duplicate in the sequence.");
			}
			sequence_.push_back(*it);
		}
	}

	copy_ = sequence_;

	delete[] t_args;
	delete[] locals;
	delete[] threads;
}
