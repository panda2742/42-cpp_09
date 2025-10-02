#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <deque>
#include <string>
#include <stdint.h>

#define THRESHOLD_THREADED_FILL 100000L
#define THREADED_FILL_CHUNK_SIZE 75000L

template <template <class T, class Alloc> class Ctn = std::deque>
class PmergeMe
{
public:
	typedef std::allocator<uint32_t>		allocator_t;
	typedef Ctn<uint32_t, allocator_t>		seq_t;
	typedef typename seq_t::iterator		it_t;
	typedef typename seq_t::const_iterator	const_it_t;

	PmergeMe(const char **seq, uint32_t seq_size, const std::string& container_name);
	~PmergeMe(void);
	PmergeMe(const PmergeMe& other);
	PmergeMe&	operator=(const PmergeMe& other);

private:
	typedef struct	fill_args
	{
		const char	**seq;
		uint32_t	seq_size;
	}	fill_args_t;

	seq_t			sequence_;
	seq_t			copy_;
	std::string	container_name_;

	template <class Class>
	void	chrono_(const std::string& message, void (Class::*f)(void *), void *f_args);
	void	fill_(void *args);
	void	ford_johnson_(void *);
	bool	is_sorted_(void) const;
	void	display_(void) const;
};

#include "PmergeMe_base.tpp"
#include "PmergeMe_impl.tpp"

#endif
