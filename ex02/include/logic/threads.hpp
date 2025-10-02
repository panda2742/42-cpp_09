#ifndef LOGIC_THREADS_HPP
#define LOGIC_THREADS_HPP

#include <stdint.h> // For uint32_t

namespace threads
{
	template <class T> struct	fillchunk_args
	{
		const char	**seq;
		uint32_t	start;
		uint32_t	end;
		T			*out;
		bool		error;
	};

	unsigned short int	nb_sort_threads;

	template <class T>
	void	*fill_chunk_job(void *void_args);
}

#include "threads.tpp"

#endif
