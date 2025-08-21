#ifndef SORTABLE_HPP
#define SORTABLE_HPP

#include <cstdlib>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <errno.h>
#include <pthread.h>
#include "types.hpp"

#define THREAD_THRESHOLD 100000
#define MIN_BLOCK 20000
#define MIN_PAIRS 4096

static pthread_mutex_t	g_jac_mutex = PTHREAD_MUTEX_INITIALIZER;

template <template <class T, class Alloc> class Ctn = std::deque> struct s_thread_args
{
	typedef Ctn<uint64_t, std::allocator<uint64_t> >	seq_t;

	const char		**seq;
	unsigned int	start;
	unsigned int	end;
	seq_t			*out;
	bool			error;
};

template <template <class T, class Alloc> class Ctn = std::deque> struct s_recur_args
{
	ASortable				*self;
	ASortable::pair_seq_t	*pairs;
	bool					is_odd;
	uint64_t				isolated;
	unsigned int			depth;
	ASortable::seq_t		*out;
};

template <template <class T, class Alloc> class Ctn = std::deque> class ASortable
{
	public:
		typedef Ctn<uint64_t, std::allocator<uint64_t> >	seq_t;
		typedef Ctn<
			std::pair<uint64_t, uint64_t>,
			std::allocator<std::pair<uint64_t, uint64_t> >
		>													pair_seq_t;
		typedef Ctn<bool, std::allocator<bool> >			bool_seq_t;
		typedef std::map<
			uint64_t,
			Ctn<uint64_t, std::allocator<uint64_t> >
		>													cache_t;
		typedef typename seq_t::iterator					it_t;
		typedef typename seq_t::const_iterator				const_it_t;

		class SortableInvalidElement : public std::exception
		{
			virtual const char	*what(void) const throw();
		};

		ASortable(void);
		ASortable(const ASortable & other);
		virtual ~ASortable(void);
		ASortable &	operator=(const ASortable & other);

		const seq_t &	GetSequence(void) const;
		void			Fill(const char **seq, uint64_t seq_size);
		virtual seq_t &	Sort(void) = 0;

	protected:
		seq_t			__sequence_;

		virtual seq_t	__Recursion(pair_seq_t & pairs, bool is_odd, uint64_t isolated_element) = 0;
		virtual void	__JacobsthalInsert(seq_t & res, seq_t & seq_to_insert) = 0;
		virtual void	__GenerateJacobsthalIndices(seq_t & indices, uint64_t n) = 0;
		virtual it_t	__LowerBound(it_t first, it_t last, uint64_t value) = 0;

		static void		*__T_FillChunk(void *void_args);
		static void		*__T_SortingWorker(void *void_args);
};

#include "ASortable.tpp"

#endif
