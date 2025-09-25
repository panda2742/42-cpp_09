#include "Utils.hpp"

#include <string>

using namespace std;

template <typename T>
string	Utils::FormatNumber(const T& n)
{
	string	str = to_string(n),
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
