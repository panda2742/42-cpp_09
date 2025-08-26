#include "PMMException.hpp"

PMMException::PMMException(const std::string & err_message)
	: err_message_(RED + err_message + RESET)
{}

PMMException::~PMMException(void) throw()
{}

const char	*PMMException::what(void) const throw()
{
	return err_message_.c_str();
}
