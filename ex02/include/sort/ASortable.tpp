#include "ASortable.hpp"

template <template <class T, class Alloc> class Ctn>
const char	*ASortable<Ctn>::SortableInvalidElement::what(void) const throw()
{
	return "Invalid sequence element.";
}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::ASortable(void): __sequence_(Ctn<uint64_t, std::allocator<uint64_t> >()), __threads_depth_(THREAD_NSIZE_MAX) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::ASortable(const ASortable<Ctn> & other)
	: __sequence_(other.__sequence_), __threads_depth_(THREAD_NSIZE_MAX) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::~ASortable(void) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn> & ASortable<Ctn>::operator=(const ASortable<Ctn> & other)
{
	if (this != &other)
	{
		this->__sequence_ = other.__sequence_;
		this->__threads_depth_ = other.__threads_depth_;
	}

	return *this;
}

template <template <class T, class Alloc> class Ctn>
const typename ASortable<Ctn>::seq_t &	ASortable<Ctn>::GetSequence(void) const
{
	return __sequence_;
}

template <template <class T, class Alloc> class Ctn>
thread_nuint64_t	ASortable<Ctn>::GetThreadsDepth(void) const
{
	return __threads_depth_;
}

template <template <class T, class Alloc> class Ctn>
void	ASortable<Ctn>::Fill(const char **seq, uint64_t seq_size)
{
	for (uint64_t	i = 0; i < seq_size; i++)
	{
		std::string	str(seq[i]);
		char		*endptr;

		if (str.empty() || str[0] == '-')
			throw SortableInvalidElement();

		errno = 0;
		uint64_t	value = std::strtoul(str.c_str(), &endptr, 10);

		if (*endptr != 0 || (value == ULONG_MAX && errno == ERANGE))
			throw SortableInvalidElement();
		
		__sequence_.push_back(value);
	}
}
