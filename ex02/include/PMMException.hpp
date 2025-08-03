#ifndef PMM_EXCEPTION_HPP_
#define PMM_EXCEPTION_HPP_

#include <exception>
#include <string>

#define RED "\e[38;2;255;0;0m"
#define RESET "\e[0m"

/**
 * Represent an error during the merge-insertion algorithm.
 */
class PMMException: public std::exception
{
public:
	/**
	 * The error constructor for the Exception. Takes a string as parameter and
	 * build the error message based on the value.
	 */
	PMMException(const std::string & errMessage) throw();
	virtual ~PMMException(void) throw();
	/**
	 * Returns the error message as a C string.
	 * 
	 * @returns The error message as a C string.
	 */
	virtual const char	*what(void) const throw();
private:
	/**
	 * The error message to return.
	 */
	std::string	_errMessage;
};

#endif
