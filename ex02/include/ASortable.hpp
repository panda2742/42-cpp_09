#ifndef SORTABLE_HPP_
#define SORTABLE_HPP_

#include <cstdlib>

template <class T> class ASortable
{
public:
	class SortableInvalidElement : public std::exception
	{
		virtual const char	*what(void) const throw();	
	};

	ASortable(void) throw();
	ASortable(const ASortable<T> & other) throw();
	virtual ~ASortable(void) throw();

	ASortable<T> &	operator=(const ASortable<T> & other) throw();

	void	Fill(const char **seq, size_t seq_size) throw(std::exception);

	virtual void	Sort(void) const throw() = 0;
protected:
	T	__sequence_;
};

#endif