#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <ctime>

#include "sort/ASortable.hpp"
#include "sort/SortableDeque.hpp"
#include "PMMException.hpp"

#define GREY "\e[38;2;100;100;100m"
#define GREEN "\e[38;2;0;255;0m"
#define BLUE_SILVER "\e[38;2;230;220;255m"
#define EMERALD_GREEN "\e[38;2;80;200;120m"
#define AMBER "\e[38;2;255;191;0m"
#define RESET "\e[0m"

typedef struct timeval	timeval_t;

template <class S = SortableDeque> class PmergeMe
{
	public:
		PmergeMe(const char **seq, uint64_t seq_size, const std::string & container_name);
		template <class T> PmergeMe(const PmergeMe<T> & other);

		~PmergeMe(void);

		PmergeMe &	operator=(const PmergeMe & other);

		void	FordJohnson(void);
		void	Display(void) const;

		void	EnableTimeMeasure(void);
		void	DisableTimeMeasure(void);
		bool	IsTimeMeasureEnabled(void) const;
		S		*GetSortable(void) const;

	private:
		S			*__sortable_;
		bool		__measure_time_;
		std::string	__container_name;
		timeval_t	__tv_init_start_;
		timeval_t	__tv_init_end_;
		timeval_t	__tv_sort_start_;
		timeval_t	__tv_sort_end_;
		
		PmergeMe(void);

		bool			__IsSorted(void) const;
		static double	__GetTimeDiff(timeval_t & start, timeval_t & end);
};

#include "PmergeMe.tpp"

#endif
