#ifndef BITCOIN_EXCHANGE_HPP
# define BITCOIN_EXCHANGE_HPP

# include <map>
# include <iostream>
# include <fstream>
# include <iomanip>
# include <inttypes.h>
# include <cstdlib>

# define C_RED "\e[38;2;240;10;30m"
# define C_GREEN "\e[38;2;10;240;30m"
# define C_BLURPLE "\e[38;2;85;57;204m"
# define C_GREY "\e[38;2;50;50;50m"
# define C_RESET "\e[0m"
# define C_CLEAR "\e[2J"
# define C_HOME "\e[H"

typedef std::map< std::string, double >		ExchangeMap;
typedef std::pair< std::string, double >	ExchangePair;
typedef ExchangeMap::const_iterator			const_exchange_iterator;
typedef ExchangeMap::iterator				exchange_iterator;

/**
 * Represent the BitcoinExchange class.
 */
class BitcoinExchange
{
public:
	static std::string	dbFilenameDefault;
	/**
	 * Tests if a year is leap or not.
	 * 
	 * @param year The year to test.
	 * @returns The result of the test.
	 */
	static bool	IsLeapYear(uint16_t year) throw();
	/**
	 * Represent the structure of a date. Each value takes as less bites as they
	 * can.
	 */
	typedef struct	Date
	{
		uint16_t	year;
		uint8_t		month;
		uint8_t		day;
		bool		operator<(const struct Date & other) const;
		bool		operator==(const struct Date & other) const;
		bool		operator!=(const struct Date & other) const;
		bool		operator<=(const struct Date & other) const;
		bool		operator>(const struct Date & other) const;
		bool		operator>=(const struct Date & other) const;
	}	Date_t;
	/**
	 * Represent an exception during the process.
	 */
	class ExchangeException: public std::exception
	{
	private:
		const std::string	_errMessage;
	public:
		/**
		 * The constructor of the exception.
		 * 
		 * @param errMessage The error message.
		 */
		ExchangeException(const std::string & errMessage) throw();
		virtual ~ExchangeException(void) throw();
		/**
		 * Return the error message.
		 * 
		 * @returns The error message.
		 */
		virtual const char	*what(void) const throw();
	};
	BitcoinExchange(void) throw();
	/**
	 * The used constructor. Takes a filename as argument that provides info
	 * about the input data.
	 * 
	 * @param inputFilename The filename of the input data file.
	 */
	BitcoinExchange(const std::string & inputFilename) throw();
	/**
	 * The copy constructor.
	 * 
	 * @param other The other BitcoinExchange instance.
	 */
	BitcoinExchange(const BitcoinExchange & other) throw();
	~BitcoinExchange(void) throw();
	/**
	 * The copy assignment operator overload.
	 * 
	 * @param other The other BitcoinExchange instance.
	 * @returns A *this reference.
	 */
	BitcoinExchange &	operator=(const BitcoinExchange & other) throw();
	/**
	 * Read the database file and parse its content to set the content into the
	 * map. Returns if the reading has been successfull or not. Throws an error
	 * if something is wrong.
	 * 
	 * @throws ExchangeException if something is wrong.
	 */
	void	ParseDatabase(void) throw(ExchangeException);
	/**
	 * Read the input line per line and return the correct value for the 
	 * associated rate. Does not throw anything.
	 * 
	 * @throws ExchangeException if something is wrong.
	 */
	void	ReadInput(void) throw(ExchangeException);
private:
	std::string			_dbFilename;
	std::string			_inputFilename;
	std::ifstream		_db;
	std::ifstream		_input;
	ExchangeMap			_map;
	/**
	 * Returns whether the string respects the date format Year-Month-Day.
	 * 
	 * @param dateLiteral The date to test.
	 * @returns The result of the test.
	 * @throws ExchangeException if something is wrong.
	 */
	Date_t	_IsValidDateFormat(const std::string & dateLiteral) const 
			throw(ExchangeException);
	/**
	 * Loads a database line on the map. Throws an error if something is wrong.
	 * 
	 * @param line The line string literal to check.
	 * @throws ExchangeException if something is wrong.
	 */
	void	_SaveLine(const std::string & line) throw(ExchangeException);
	/**
	 * Read an input line and try to understands it, and to returns the
	 * calculated value.
	 * 
	 * @param line The line string literal to check.
	 * @throws ExchangeException if something is wrong.
	 */
	void	_UnderstandLine(const std::string & line) throw(ExchangeException);
	/**
	 * Get the rate of a certain date by choosing the accurate one or the lower
	 * one. Returns the found pair, with the rate and the nearest lowest date.
	 * 
	 * @param date The date to look for.
	 * @returns A pair from the map.
	 * @throws ExchangeException if something is wrong.
	 */
	ExchangePair	_GetRateForDate(Date_t date) const throw(ExchangeException);
};

#endif /* BITCOIN_EXCHANGE_HPP */
