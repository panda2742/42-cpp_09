#include "Utils.hpp"

#include <string> // string

namespace utils
{

template <typename T>
std::string	FormatNumber(const T& n)
{
	std::string	str = std::to_string(n),
			result;

	size_t	count = 0;
	for (int	i = str.length() - 1; i >= 0; i--)
	{
		if (count > 0 && count % 3 == 0)
			result = " " + result;
		result = str[i] + result;
		count++;
	}
	
	return result;
}

}
