#include "PMMException.hpp"

PMMException::PMMException(const std::string & errMessage) throw()
	: _errMessage(RED + errMessage + RESET)
{}

PMMException::~PMMException(void) throw()
{}

const char	*PMMException::what(void) const throw()
{
	return _errMessage.c_str();
}
