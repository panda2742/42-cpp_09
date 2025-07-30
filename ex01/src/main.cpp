#include <iostream>
#include "RPN.hpp"

int	main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
			throw RPN::RPNException(
				"Invalid format. Expected: ./rpn <expr>"
			);
		RPN	rpn(argv[1]);

		rpn.Operate();
	}
	catch (const RPN::RPNException & e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
