#include "BitcoinExchange.hpp"

int	main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
			throw BitcoinExchange::ExchangeException(
				"Invalid format. Expected: ./btc <*.csv>"
			);
		BitcoinExchange	btc(argv[1]);

		btc.ParseDatabase();
		btc.ReadInput();
	}
	catch (const BitcoinExchange::ExchangeException & e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
