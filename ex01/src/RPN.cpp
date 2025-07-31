#include "RPN.hpp"
#include <ctype.h>
#include <iostream>

static int	isoper(const char __c)
{
	return __c == '+' || __c == '-' || __c == '/' || __c == '*';
}

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

RPN::RPN(const std::string & expr) throw(): _expr(expr)
{}

RPN::RPN(const RPN & other) throw(): _expr(other._expr), _stack(other._stack)
{}

RPN::~RPN(void) throw()
{}

RPN & RPN::operator=(const RPN & other) throw()
{
	if (this != &other)
	{
		this->_expr = other._expr;
		this->_stack = other._stack;
	}

	return *this;
}

void	RPN::Operate(void) throw(RPN::RPNException)
{
	const std::string	op = "+-/*";
	for (std::string::iterator	it = _expr.begin(); it < _expr.end(); it++)
	{
		char	c = *it;
		if (isspace(c)) continue;
		else if (isdigit(c))
		{
			_stack.push(c - '0');
			_debug_list.push_back(c - '0');
		}
		else if (isoper(c))
		{
			if (_stack.size() < 2)
				throw RPNException(C_RED "Cannot operate on a single sized stack." C_RESET);

			double	elt[2];
			elt[0] = _stack.top();
			_stack.pop();
			elt[1] = _stack.top();
			_stack.pop();

			switch (c)
			{
				case '+':
					_stack.push(elt[1] + elt[0]);
					break;
				case '-':
					_stack.push(elt[1] - elt[0]);
					break;
				case '/':
					_stack.push(elt[1] / elt[0]);
					break;
				case '*':
					_stack.push(elt[1] * elt[0]);
					break;
				default:
					break;
			}
		}
		else
		{
			throw RPNException(C_RED "Error. Invalid char `" + std::string(&c).substr(0, 1) + "'." C_RESET);
			return ;
		}
	}
	// RPN::Display();
	std::cout << C_GREEN << _stack.top() << C_RESET << std::endl;
}

void	RPN::Display(void) const throw()
{
	for (
		RPNList::const_iterator	it = _debug_list.begin();
		it != _debug_list.end();
		it++
	)
	{
		std::cout << C_BLURPLE << *it << " ";
	}
	std::cout << C_RESET << std::endl;
}
