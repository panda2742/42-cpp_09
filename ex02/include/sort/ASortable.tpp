#include "ASortable.hpp"
#include <string>
#include <sstream>
#include <pthread.h>
#include "utils.hpp"
#include <iostream>

template <template <class T, class Alloc> class Ctn>
const char	*ASortable<Ctn>::SortableInvalidElement::what(void) const throw()
{
	return "Invalid sequence element (duplication or invalid type).";
}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::ASortable(void): __sequence_(Ctn<uint64_t, std::allocator<uint64_t> >()) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::ASortable(const ASortable<Ctn> & other)
	: __sequence_(other.__sequence_) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn>::~ASortable(void) {}

template <template <class T, class Alloc> class Ctn>
ASortable<Ctn> & ASortable<Ctn>::operator=(const ASortable<Ctn> & other)
{
	if (this != &other)
		this->__sequence_ = other.__sequence_;

	return *this;
}

template <template <class T, class Alloc> class Ctn>
const typename ASortable<Ctn>::seq_t &	ASortable<Ctn>::GetSequence(void) const
{
	return __sequence_;
}

template <template <class T, class Alloc> class Ctn>
void	ASortable<Ctn>::Fill(const char **seq, uint64_t seq_size)
{
	std::string	check_duplicates;

	Ctn<std::string, std::allocator<std::string> >	duplicates;
	unsigned int	max_threads = get_hardware_concurrency();

	std::cout << "MAX CONCURRENCY IS " << max_threads << std::endl;
	for (uint64_t	i = 0; i < seq_size; i++)
	{
		std::string	str(seq[i]);
		char		*endptr;

		if (str.empty() || str[0] == '-') throw SortableInvalidElement();

		errno = 0;
		uint64_t	value = std::strtoul(str.c_str(), &endptr, 10);

		std::ostringstream	val_stream;
		val_stream << "," << value << ",";
		const std::string	val_str = val_stream.str();	

		if (
			*endptr != 0 ||
			(value == ULONG_MAX && errno == ERANGE) ||
			check_duplicates.find(val_str, 0) != std::string::npos
		) throw SortableInvalidElement();

		check_duplicates.append(val_str);
		__sequence_.push_back(value);
	}
}
