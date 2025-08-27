#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <ctime>
#include <cstdlib>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <errno.h>
#include <pthread.h>
#include "types.hpp"

#define GREEN "\e[38;2;0;255;0m"
#define RED "\e[38;2;255;0;0m"
#define RESET "\e[0m"

typedef struct timeval	timeval_t;

template <template <class T, class Alloc> class Ctn = std::deque>
class PmergeMe
{
	public:
		typedef std::allocator<uint64_t>		Allocator;
		typedef Ctn<uint64_t, Allocator>		Seq;
		typedef std::pair<uint64_t, uint64_t>	Pair;
		typedef std::allocator<Pair>			PairAllocator;
		typedef Ctn<Pair, PairAllocator>		PairSeq;
		typedef std::allocator<bool>			BoolAllocator;
		typedef Ctn<bool, BoolAllocator>		BoolSeq;
		typedef std::map<uint64_t, Seq>			Cache;
		typedef typename Seq::iterator			It;
		typedef typename Seq::const_iterator	ConstIt;

		PmergeMe(const char **seq, uint64_t seq_size, const std::string & container_name);
		PmergeMe(const PmergeMe & other);
		~PmergeMe(void);
		PmergeMe &	operator=(const PmergeMe & other);

		const Seq &			GetSequence(void) const;
		const Seq &			GetCopy(void) const;
		bool				IsTimeMeasured(void) const;
		const std::string &	GetContainerName(void) const;

		void				EnableTimeMeasure(void);
		void				DisableTimeMeasure(void);

		void				FordJohnson(void);
		void				Display(void) const;

	private:
		Seq			sequence_;
		Seq			copy_;
		bool		measure_time_;
		std::string	container_name_;
		timeval_t	tv_init_start_;
		timeval_t	tv_init_end_;
		timeval_t	tv_sort_start_;
		timeval_t	tv_sort_end_;

		PmergeMe(void);

		static double	GetTimeDiff_(timeval_t & start, timeval_t & end);

		void			Fill_(const char **seq, uint64_t seq_size);
		bool			IsSorted_(void) const;

};

#include "PmergeMe_base.tpp"
#include "PmergeMe_impl.tpp"

#endif
