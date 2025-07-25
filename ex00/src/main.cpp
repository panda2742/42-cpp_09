#include "BitcoinExchange.hpp"

int	main(int argc, char **argv)
{
	BitcoinExchange	btc;

	try
	{
		btc.ParseDatabase();
		btc.ReadInput();
	}
	catch (const BitcoinExchange::ExchangeException & e)
	{
		std::cout << e.what() << std::endl;
	}
	(void)argc;
	(void)argv;
	return 0;
}
