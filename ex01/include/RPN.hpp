#ifndef RPN_HPP
# define RPN_HPP

# include <string>
# include <stack>
# include <list>
# include <inttypes.h>

# define C_RED "\e[38;2;240;10;30m"
# define C_GREEN "\e[38;2;10;240;30m"
# define C_BLURPLE "\e[38;2;85;57;204m"
# define C_GREY "\e[38;2;50;50;50m"
# define C_RESET "\e[0m"
# define C_CLEAR "\e[2J"
# define C_HOME "\e[H"

typedef std::stack< float, std::list< float > >	RPNStack;

/**
 * Represent the RPN class.
 */
class RPN
{
public:
	/**
	 * Represent an exception during the process.
	 */
	class RPNException: public std::exception
	{
	private:
		const std::string	_errMessage;
	public:
		/**
		 * The constructor of the exception.
		 * 
		 * @param errMessage The error message.
		 */
		RPNException(const std::string & errMessage) throw();
		virtual ~RPNException(void) throw();
		/**
		 * Return the error message.
		 * 
		 * @returns The error message.
		 */
		virtual const char	*what(void) const throw();
	};
	RPN(void) throw();
	/**
	 * The string constructor. Takes a RPN expression as a parameter.
	 * Throws an error if something is wrong with the expression.
	 * 
	 * @param expr The expression to evaluate.
	 * @throws RPNException if something is wrong.
	 */
	RPN(const std::string & expr) throw(RPNException);
	/**
	 * The copy constructor. Takes another instance of RPN as parameter and
	 * creates a copy of it.
	 * 
	 * @param other The other instance of RPN.
	 */
	RPN(const RPN & other) throw();
	~RPN(void) throw();
	/**
	 * The copy assignment operator overload. Takes another RPN instance
	 * as parameter and creates a copy of it.
	 * 
	 * @param other The other instance
	 * @returns A *this reference.
	 */
	RPN & operator=(const RPN & other) throw();
	/**
	 * Launch the operations on the stack and returns the result of the
	 * expression.
	 * 
	 * @throws RPNExpression if needed.
	 */
	void	Operate(void) throw(RPNException);
private:
	const std::string	_expr;
	RPNStack			_stack;
};

#endif /* RPN_HPP */