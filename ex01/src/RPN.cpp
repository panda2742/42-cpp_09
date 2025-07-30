#include "RPN.hpp"

RPN::
	RPNException::RPNException(const std::string & errMessage) throw():
		_errMessage(C_RED + errMessage + C_RESET)
	{}

RPN::
	RPNException::~RPNException(void) throw()
	{}

const char	*RPN::RPNException::what(void) const throw()
{
	return _errMessage.c_str();
}

RPN::RPN(void) throw(): _expr("")
{}

RPN::RPN(const std::string & expr) throw(RPN::RPNException)
{

}

RPN::RPN(const RPN & other) throw()
{

}

RPN::~RPN(void) throw()
{}

RPN & RPN::operator=(const RPN & other) throw()
{

}

void	RPN::Operate(void) throw(RPN::RPNException)
{}
