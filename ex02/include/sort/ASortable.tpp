#include "ASortable.hpp"
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include "utils.hpp"

template <template <class T, class Alloc> class Ctn>
const char	*ASortable<Ctn>::SortableInvalidElement::what(void) const throw()
{
	return "Invalid sequence element (duplication or invalid type).";
}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::ASortable(void): __sequence_(Ctn<uint64_t, std::allocator<uint64_t> >()) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::ASortable(const ASortable<Ctn> & other)
	: __sequence_(other.__sequence_) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::~ASortable(void) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn> & ASortable<Ctn>::operator=(const ASortable<Ctn> & other)
{
	if (this != &other)
		this->__sequence_ = other.__sequence_;

	return *this;
}

template <template <class T, class Alloc> class Ctn>
const typename ASortable<Ctn>::seq_t &	ASortable<Ctn>::GetSequence(void) const
{
	return __sequence_;
}

template <template <class T, class Alloc> class Ctn>
void	ASortable<Ctn>::Fill(const char **seq, uint64_t seq_size)
{
	if (seq_size == 0)
	{
		__sequence_.clear();
		return;
	}

	if (seq_size < THREAD_THRESHOLD)
	{
		std::set<uint64_t>	seen;
		for (uint64_t	i = 0; i < seq_size; i++)
		{
			char		*endptr;
			if (strlen(seq[i]) == 0 || seq[i][0] == '-') throw SortableInvalidElement();
			errno = 0;
			uint64_t	value = std::strtoul(seq[i], &endptr, 10);
			if (
				*endptr != 0 ||
				(value == ULONG_MAX && errno == ERANGE) ||
				!seen.insert(value).second
			) throw SortableInvalidElement();
			__sequence_.push_back(value);
		}
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
	s_thread_args<Ctn>	*args = new s_thread_args<Ctn>[nthreads];
	seq_t				*locals = new seq_t[nthreads];

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
		pthread_create(&threads[t], NULL, __T_FillChunk, &args[t]);
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
		throw SortableInvalidElement();
	}

	__sequence_.clear();
	std::set<uint64_t>	seen;
	for (t = 0; t < nthreads; t++)
	{
		for (const_it_t	it = locals[t].begin(); it != locals[t].end(); it++)
		{
			if (!seen.insert(*it).second)
			{
				delete[] threads; delete[] args; delete[] locals;
				throw SortableInvalidElement();
			}
			__sequence_.push_back(*it);
		}
	}

	delete[] threads;
	delete[] args;
	delete[] locals;
}

template <template <class T, class Alloc> class Ctn>
void	*ASortable<Ctn>::__T_FillChunk(void *void_args)
{
	s_thread_args<Ctn>	*args = reinterpret_cast<s_thread_args<Ctn> *>(void_args);
	args->error = false;

	for (uint64_t	i = args->start; i < args->end; i++)
	{
		if (args->seq[i][0] == '\0' || args->seq[i][0] == '-')
		{
			args->error = true;
			return NULL;
		}

		char	*endptr;
		errno = 0;
		uint64_t	value = std::strtoul(args->seq[i], &endptr, 10);

		if (*endptr != '\0' || (value == ULONG_MAX && errno == ERANGE))
		{
			args->error = true;
			return NULL;
		}

		args->out->push_back(value);
	}

	return NULL;
}
