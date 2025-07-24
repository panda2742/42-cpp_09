#ifndef BITCOIN_EXCHANGE_HPP
# define BITCOIN_EXCHANGE_HPP

# include <map>
# include <iostream>
# include <fstream>

/**
 * Represent the BitcoinExchange class.
 */
class BitcoinExchange
{
private:
	/**
	 * The filename of the Bitcoin database.
	 */
	std::string						_dbFilename;
	/**
	 * The filename of the input data to analys.
	 */
	std::string						_inputFilename;
	/**
	 * The database file stream.
	 */
	std::fstream					_db;
	/**
	 * The input data file stream.
	 */
	std::fstream					_input;
	/**
	 * The map containing all the Bitcoin data.
	 */
	std::map<std::string, float>	_map;
public:
	/**
	 * The default filename for the database filename.
	 */
	static std::string	dbFilenameDefault;
	/**
	 * The default constructor.
	 */
	BitcoinExchange(void);
	/**
	 * The used constructor. Takes a filename as argument that provides info
	 * about the input data.
	 * 
	 * @param inputFilename The filename of the input data file.
	 */
	BitcoinExchange(const std::string & inputFilename);
	/**
	 * The copy constructor.
	 * 
	 * @param other The other BitcoinExchange instance.
	 */
	BitcoinExchange(const BitcoinExchange & other);
	/**
	 * The default destructor.
	 */
	~BitcoinExchange(void);
	/**
	 * The copy assignment operator overload.
	 * 
	 * @param other The other BitcoinExchange instance.
	 * @returns A *this reference.
	 */
	BitcoinExchange & operator=(const BitcoinExchange & other);
};

#endif /* BITCOIN_EXCHANGE_HPP */
