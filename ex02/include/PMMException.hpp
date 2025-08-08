#ifndef PMM_EXCEPTION_HPP
#define PMM_EXCEPTION_HPP

#include <exception>
#include <string>

#define RED "\e[38;2;255;0;0m"
#define RESET "\e[0m"

class PMMException: public std::exception
{
	public:
		PMMException(const std::string & errMessage) throw();
		virtual ~PMMException(void) throw();
		virtual const char	*what(void) const throw();

	private:
		std::string	_errMessage;
};

#endif
