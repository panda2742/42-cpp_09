#include "threads.hpp"

#include <errno.h> // For errno
#include <string> // For string
#include <cstdlib> // For strtoul

namespace threads
{
	template <class T>
	void	*fill_chunk_job(void *void_args)
	{
		fillchunk_args<T>	*args = reinterpret_cast<fillchunk_args<T> *>(void_args);
		args->error = false;

		for (uint32_t i = args->start; i < args->end; ++i)
		{
			if (args->seq[i][0] == '\0' || args->seq[i][0] == '-')
			{
				args->error = true;
				return NULL;
			}

			errno = 0;
			char		*endptr;
			uint32_t	value = std::strtoul(args->seq[i], &endptr, 10);

			if (*endptr != '\0' || (value == 0xffffffff && errno == ERANGE))
			{
				args->error = true;
				return NULL;
			}
			args->out->push_back(value);
		}

		return NULL;
	}
}