#ifndef SORTABLE_HPP
#define SORTABLE_HPP

#include <inttypes.h>

#include <cstdlib>
#include <deque>

typedef std::deque<uint64_t>	sortable_deque_t;
typedef std::deque< std::pair<uint64_t, uint64_t> >	deque_pair_t;

template <class T = sortable_deque_t, class P = deque_pair_t> class ASortable
{
	public:
		typedef std::deque<bool>	Base;

		typename T::iterator	begin(void) { return __sequence_.begin(); }
		typename T::const_iterator	begin(void) const { return __sequence_.begin(); }
		typename T::iterator	end(void) { return __sequence_.begin(); }
		typename T::const_iterator	end(void) const { return __sequence_.begin(); }

		class SortableInvalidElement : public std::exception
		{
			virtual const char	*what(void) const throw();
		};

		ASortable(void);
		ASortable(const ASortable & other);
		virtual ~ASortable(void);

		T &	GetSequence(void);
		const T &	GetSequence(void) const;

		virtual ASortable &	operator=(const ASortable & other);

		void					Fill(const char **seq, size_t seq_size);
		virtual T &				Sort(void) = 0;

	protected:
		T	__sequence_;

		virtual T				__Recursion(P & pairs, bool is_odd, uint64_t isolated_element) = 0;
		void					__GenerateJacobsthalIndices(T & indices, size_t n) const;
		void					__JacobsthalInsert(T & res, T & seq_to_insert) const;
		typename T::iterator	__LowerBound(typename T::iterator first, typename T::iterator last, uint64_t value) const;
};

#include "ASortable.tpp"

#endif
