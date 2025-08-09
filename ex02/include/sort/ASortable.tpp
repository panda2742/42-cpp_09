#include "ASortable.hpp"

#include <climits>
#include <cstdlib>
#include <string>
#include <errno.h>

template <class T, class P>
const char	*ASortable<T, P>::SortableInvalidElement::what(void) const throw()
{
	return "Invalid sequence element.";
}

template <class T, class P>
ASortable<T, P>::ASortable(void): __sequence_(T()) {}

template <class T, class P>
ASortable<T, P>::ASortable(const ASortable<T, P> & other): __sequence_(other.__sequence_) {}

template <class T, class P>
ASortable<T, P>::~ASortable(void) {}

template <class T, class P>
ASortable<T, P> & ASortable<T, P>::operator=(const ASortable<T, P> & other)
{
	if (this != &other)
		this->__sequence_ = other.__sequence_;

	return *this;
}

template <class T, class P>
T &	ASortable<T, P>::GetSequence(void)
{
	return __sequence_;
}

template <class T, class P>
const T &	ASortable<T, P>::GetSequence(void) const
{
	return __sequence_;
}

template <class T, class P>
void	ASortable<T, P>::Fill(const char **seq, size_t seq_size)
{
	for (size_t	i = 0; i < seq_size; i++)
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

template <class T, class P>
void	ASortable<T, P>::__GenerateJacobsthalIndices(T & indices, size_t n) const
{
	indices.clear();
	if (n == 0)
		return;

	T	jacobsthal;
	jacobsthal.push_back(1);
	if (n > 1)
		jacobsthal.push_back(1);

	while (jacobsthal.back() < n)
	{
		uint64_t	next = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
		if (next > n)
			break;
		jacobsthal.push_back(next);
	}

	Base	used;
	for (size_t	k = 0; k < n; k++)
		used.push_back(false);

	if (n > 0)
	{
		indices.push_back(0);
		used[0] = true;
	}

	for (size_t	i = 2; i < jacobsthal.size(); i++)
	{
		size_t	start = jacobsthal[i] - 1,
				end = jacobsthal[i - 1];

		for (size_t	j = start; j >= end && j < n; j--)
		{
			if (!used[j])
			{
				indices.push_back(j);
				used[j] = true;
			}
			if (j == 0)
				break;
		}
	}

	for (size_t	i = 0; i < n; i++)
	{
		if (!used[i])
			indices.push_back(i);
	}
}

template <class T, class P>
void	ASortable<T, P>::__JacobsthalInsert(T & res, T & seq_to_insert) const
{
	if (seq_to_insert.empty())
		return;
	
	T	jacobsthal_indices;
	__GenerateJacobsthalIndices(jacobsthal_indices, seq_to_insert.size());

	for (size_t	i = 0; i < jacobsthal_indices.size(); i++)
	{
		uint64_t	idx = jacobsthal_indices[i];
		
		if (idx < seq_to_insert.size())
		{
			uint64_t				element = seq_to_insert[idx];
			typename T::iterator	pos = __LowerBound(res.begin(), res.end(), element);
			res.insert(pos, element);
		}
	}
}

template <class T, class P>
typename T::iterator	ASortable<T, P>::__LowerBound(typename T::iterator first, typename T::iterator last, uint64_t value) const
{
	typename T::iterator	it, temp = first;
	size_t	count = 0, step;

	while (temp != last)
	{
		++temp;
		++count;
	}

	while (count > 0)
	{
		it = first;
		step = count / 2;

		for (size_t	i = 0; i < step; i++)
			it++;
		
		if (*it < value)
		{
			first = it;
			++first;
			count -= step + 1;
		}
		else count = step;
	}

	return first;
}
