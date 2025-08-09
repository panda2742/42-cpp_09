#include "PMMException.hpp"

PMMException::PMMException(const std::string & err_message)
	: __err_message_(RED + err_message + RESET)
{}

PMMException::~PMMException(void) throw()
{}

const char	*PMMException::what(void) const throw()
{
	return __err_message_.c_str();
}
