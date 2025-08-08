#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <ctime>

#include "sort/ASortable.hpp"
#include "sort/DequeSortable.hpp"
#include "PMMException.hpp"

#define GREY "\e[38;2;100;100;100m"
#define RESET "\e[0m"

typedef struct timeval	timeval_t;

template <class S = DequeSortable> class PmergeMe
{
	public:
		PmergeMe(const char **seq, size_t seq_size) throw(typename S::SortableInvalidElement);
		template <class T> PmergeMe(const PmergeMe<T> & other) throw();

		~PmergeMe(void) throw();

		PmergeMe &	operator=(const PmergeMe & other) throw();

		void	FordJohnson(void) throw();
		void	Display(void) const throw();

		void	EnableTimeMeasure(void) throw();
		void	DisableTimeMeasure(void) throw();
		bool	IsTimeMeasureEnabled(void) const throw();
		S		*GetSortable(void) const throw();

	private:
		S			*_sortable;
		bool		_measure_time;
		timeval_t	_tv_init_start;
		timeval_t	_tv_init_end;
		timeval_t	_tv_sort_start;
		timeval_t	_tv_sort_end;
		
		PmergeMe(void) throw(PMMException);
};

#include "PmergeMe.tpp"

#endif
