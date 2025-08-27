#include "PmergeMe.hpp"
#include "Algorithm.hpp"
#include "inttypes.h"
#include <set>
#include <cstring>
#include "utils.hpp"

// ===========================================================
// |                     PUBLIC MEMBERS                      |
// ===========================================================

template <template <class T, class Alloc> class Ctn>
const typename PmergeMe<Ctn>::Seq &	PmergeMe<Ctn>::GetSequence(void) const
{
	return sequence_;
}

template <template <class T, class Alloc> class Ctn>
const typename PmergeMe<Ctn>::Seq &	PmergeMe<Ctn>::GetCopy(void) const
{
	return copy_;
}

template <template <class T, class Alloc> class Ctn>
bool	PmergeMe<Ctn>::IsTimeMeasured(void) const
{
	return measure_time_;
}

template <template <class T, class Alloc> class Ctn>
const std::string &	PmergeMe<Ctn>::GetContainerName(void) const
{
	return container_name_;
}

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::EnableTimeMeasure(void)
{
	measure_time_ = true;
}

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::DisableTimeMeasure(void)
{
	measure_time_ = false;
}

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::FordJohnson(void)
{
	if (measure_time_ && gettimeofday(&tv_sort_start_, NULL) == -1)
		measure_time_ = false;

	sequence_ = Algorithm<PmergeMe<Ctn> >::Sort(sequence_);

	if (measure_time_ && gettimeofday(&tv_sort_end_, NULL) == -1)
		measure_time_ = false;
	
	Display();

	if (measure_time_)
	{
		double	time_res = GetTimeDiff_(tv_sort_start_, tv_sort_end_);

		std::cout << std::fixed << std::setprecision(3);
		std::cout << "[" << container_name_ << "] Sorting: " << time_res << "μs (~" << time_res / 1000L
			<< "ms, ~" << time_res / 1000000L << "s)." << std::endl;
	}
}

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::Display(void) const
{
	std::cout << "Sequence data: " << (IsSorted_() ? GREEN "sorted" : RED "not sorted");
	std::cout << RESET "\n";
	for (ConstIt	it = sequence_.begin(); it != sequence_.end(); it++)
	{
		if (it != sequence_.begin())
			std::cout << "  ";
		std::cout << *it;
	}
	std::cout << std::endl;
}

// ===========================================================
// |                     PRIVATE MEMBERS                     |
// ===========================================================

template <template <class T, class Alloc> class Ctn>
double	PmergeMe<Ctn>::GetTimeDiff_(timeval & start, timeval & end)
{
	return (end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec - start.tv_usec;
}

template <template <class T, class Alloc> class Ctn>
bool	PmergeMe<Ctn>::IsSorted_(void) const
{
	if (sequence_.size() < 2)
		return true && copy_.size() == sequence_.size();

	for (ConstIt	it = sequence_.begin(); (it + 1) != sequence_.end(); it++)
	{
		if (*it > *(it + 1))
			return false;
	}
	return true && copy_.size() == sequence_.size();
}

template <template <class T, class Alloc> class Ctn>
void	PmergeMe<Ctn>::Fill_(const char **seq, uint64_t seq_size)
{
	if (seq_size == 0)
	{
		sequence_.clear();
		return;
	}

	if (seq_size < THREAD_THRESHOLD)
	{
		std::cout << "Sequence size: " << seq_size << " < THREAD_THRESHOLD, no threads." << std::endl;
		std::set<uint64_t>	seen;
		for (uint64_t	i = 0; i < seq_size; i++)
		{
			char		*endptr;
			if (strlen(seq[i]) == 0 || seq[i][0] == '-') throw std::runtime_error("Invalid element format.");
			errno = 0;
			uint64_t	value = std::strtoul(seq[i], &endptr, 10);
			if (
				*endptr != 0 ||
				(value == ULONG_MAX && errno == ERANGE) ||
				!seen.insert(value).second
			) throw std::runtime_error("Invalid element format.");
			sequence_.push_back(value);
		}
		copy_ = sequence_;
		return;
	}

	unsigned int	max_threads = get_hardware_concurrency();
	if (max_threads == 0) max_threads = 2;

	uint64_t	potential = seq_size / MIN_BLOCK;
	if (potential == 0) potential = 1;

	unsigned int		nthreads = static_cast<unsigned int>(potential);
	if (nthreads > max_threads) nthreads = max_threads;
	else if (nthreads > seq_size) nthreads = static_cast<unsigned int>(seq_size);

	uint64_t	block = (seq_size + nthreads - 1) / nthreads;

	pthread_t			*threads = new pthread_t[nthreads];
	s_thread_args<Seq>	*args = new s_thread_args<Seq>[nthreads];
	Seq					*locals = new Seq[nthreads];
	
	std::cout << "Sequence size: " << seq_size << ", " << nthreads << " threads for initialization." << std::endl;

	unsigned int	t = 0;
	for (; t < nthreads; t++)
	{
		uint64_t	start = static_cast<uint64_t>(t) * block,
					end = start + block;
		if (start >= seq_size) start = end = seq_size;
		if (end > seq_size) end = seq_size;

		args[t].seq		= seq;
		args[t].start	= start;
		args[t].end		= end;
		args[t].out		= &locals[t];
		args[t].error	= false;
		pthread_create(&threads[t], NULL, Algorithm<PmergeMe<Ctn> >::T_FillChunk, &args[t]);
	}

	bool	error = false;
	for (t = 0; t < nthreads; t++)
	{
		pthread_join(threads[t], NULL);
		if (args[t].error) error = true;
	}
	if (error)
	{
		delete[] threads; delete[] args; delete[] locals;
		throw std::runtime_error("Invalid element format.");
	}

	sequence_.clear();
	std::set<uint64_t>	seen;
	for (t = 0; t < nthreads; t++)
	{
		for (ConstIt	it = locals[t].begin(); it != locals[t].end(); it++)
		{
			if (!seen.insert(*it).second)
			{
				delete[] threads; delete[] args; delete[] locals;
				throw std::runtime_error("Invalid element format.");
			}
			sequence_.push_back(*it);
		}
	}

	copy_ = sequence_;

	delete[] args;
	delete[] locals;
	delete[] threads;
}
