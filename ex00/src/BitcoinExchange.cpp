#include "BitcoinExchange.hpp"

std::string	BitcoinExchange::dbFilenameDefault = "data.csv";

bool	BitcoinExchange::IsLeapYear(uint16_t year) throw()
{
	if (year % 4 == 0)
	{
		if (year % 100 == 0)
			return (year % 400);
		else
			return true;
	}
	return false;
}

bool	BitcoinExchange::Date_t::operator<(const Date_t & other) const
{
	if (year < other.year) return true;
	if (year > other.year) return false;

	if (month < other.month) return true;
	if (month > other.month) return false;

	if (day < other.day) return true;
	if (day > other.day) return false;

	return false;
}

bool	BitcoinExchange::Date_t::operator==(const Date_t & other) const
{
	return (year == other.year && month == other.month && day == other.day);
}

bool	BitcoinExchange::Date_t::operator!=(const Date_t & other) const
{
	return !(*this == other);
}

bool	BitcoinExchange::Date_t::operator<=(const Date_t & other) const
{
	return *this < other || *this == other;
}

bool	BitcoinExchange::Date_t::operator>(const Date_t & other) const
{
	return !(*this <= other);
}

bool	BitcoinExchange::Date_t::operator>=(const Date_t & other) const
{
	return *this > other || *this == other;
}

BitcoinExchange::
	ExchangeException::ExchangeException(const std::string & errMessage) 
	throw(): _errMessage(C_RED + errMessage + C_RESET)
	{}

BitcoinExchange::
	ExchangeException::~ExchangeException(void) throw()
	{}

const char	*BitcoinExchange::ExchangeException::what(void) const throw()
{
	return _errMessage.c_str();
}

BitcoinExchange::BitcoinExchange(void) throw():
	_dbFilename(dbFilenameDefault), _inputFilename("input.csv")
{}

BitcoinExchange::BitcoinExchange(const std::string & inputFilename) throw():
	_dbFilename(dbFilenameDefault), _inputFilename(inputFilename)
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange & other) throw():
	_dbFilename(other._dbFilename),
	_inputFilename(other._inputFilename),
	_map(other._map)
{}

BitcoinExchange::~BitcoinExchange(void) throw()
{
	_map.clear();
}

BitcoinExchange &	BitcoinExchange::operator=(const BitcoinExchange & other)
throw()
{
	if (this != &other)
	{
		this->_dbFilename = other._dbFilename;
		this->_inputFilename = other._inputFilename;
		this->_map = other._map;
	}

	return *this;
}

BitcoinExchange::Date_t	BitcoinExchange::_IsValidDateFormat(
	const std::string & dateLiteral
)
const throw(ExchangeException)
{
	if (dateLiteral.length() != 10)
		throw ExchangeException("The date literal length must be 10.");

	for (size_t	i = 0; i < dateLiteral.length(); i++)
	{
		if (i == 4 || i == 7)
		{
			if (dateLiteral[i] == '-') continue;
			throw ExchangeException(
				"The format of the date must be YYYY-MM-DD."
			);
		}
		if (!isdigit(dateLiteral[i])) throw ExchangeException(
			"The format of the date must be YYYY-MM-DD."
		);
	}

	Date_t	date = {
		.year = uint16_t(atoi(dateLiteral.substr(0, 4).c_str())),
		.month = uint8_t(atoi(dateLiteral.substr(5, 2).c_str())),
		.day = uint8_t(atoi(dateLiteral.substr(8, 2).c_str())),
	};

	if (date.year < 2009 || date.year > 2025)
		throw ExchangeException(
			"Year value must be greater than or equal 2009."
		);
	if (date.month < 1 || date.month > 12)
		throw ExchangeException(
			"Month value must be between 1 and 12."
		);
	switch (date.month)
	{
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			if (date.day < 1 || date.day > 31)
				throw ExchangeException(
					"Day value must be between 1 and 31 for this month."
				);
			break;
		case 2:
			if (date.day < 1 || (IsLeapYear(date.year) && date.day > 29))
				throw ExchangeException(
					"Day value must be between 1 and 29 for this month."
				);
			else if (date.day < 1 || (!IsLeapYear(date.year) && date.day > 28))
				throw ExchangeException(
					"Day value must be between 1 and 28 for this month."
				);
			break;
		case 4: case 6: case 9: case 11:
			break;
		default:
			throw ExchangeException("Unknown day.");
	}

	return date;
}

void	BitcoinExchange::_SaveLine(const std::string & line)
throw(ExchangeException)
{
	std::cout << C_GREY "Saving '" << line << "'" C_RESET " ";

	size_t	separatorPos = line.find(',');
	if (separatorPos == std::string::npos)
	{
		std::cout << "❌" << std::endl;
		throw ExchangeException(
			"Invalid format for line. Expected `date,value`."
		);
	}

	const std::string	dateStr = line.substr(0, separatorPos);
	const std::string	valueStr = line.substr(separatorPos + 1);
	_IsValidDateFormat(dateStr);

	char				*endPtr;
	const double		value = strtod(valueStr.c_str(), &endPtr);

	if (endPtr == valueStr.c_str() || *endPtr != '\0')
	{
		std::cout << "❌" << std::endl;
		throw ExchangeException("The value is not a double.");
	}

	if (value < 0)
	{
		std::cout << "❌" << std::endl;
		throw ExchangeException("The value cannot be negative.");
	}

	_map.insert(ExchangePair(dateStr, value));
	std::cout << "✅" << std::endl;
}

void	BitcoinExchange::_UnderstandLine(const std::string & line)
throw (BitcoinExchange::ExchangeException)
{
	if (!line.length())
	{
		std::cout << C_GREY << "(Empty line)" C_RESET << std::endl;
		return;
	}

	std::cout << std::left << std::setw(20) << line << " => ";

	size_t	separatorPos = line.find('|');
	if (separatorPos == std::string::npos)
	{
		std::cout << std::endl << "❌ ";
		throw ExchangeException(
			"Invalid format for line. Expected `date | value`."
		);
	}

	std::string		dateStr = line.substr(0, separatorPos);
	std::string		valueStr = line.substr(separatorPos + 1);

	if (dateStr[dateStr.length() - 1] != ' ')
	{
		std::cout << std::endl << "❌ ";
		throw ExchangeException("Invalid format for line. Missing space.");
	}
	dateStr = dateStr.substr(0, dateStr.length() - 1);
	Date_t	date;
	try
	{
		date = _IsValidDateFormat(dateStr);
	}
	catch (const ExchangeException & e)
	{
		std::cout << std::endl << "❌ ";
		throw;
	}
	if (valueStr[0] != ' ')
	{
		std::cout << std::endl << "❌ ";
		throw ExchangeException("Invalid format for line. Missing space.");
	}
	valueStr = valueStr.substr(1);

	char			*endPtr;
	const double	value = strtod(valueStr.c_str(), &endPtr);

	if (endPtr == valueStr.c_str() || *endPtr != '\0')
	{
		std::cout << std::endl << "❌ ";
		throw ExchangeException("The value is not a double.");
	}

	if (value < 0 || value > 1000)
	{
		std::cout << std::endl << "❌ ";
		throw ExchangeException(
			"The value cannot be negative or greater than 1000."
		);
	}
	std::cout << std::right << std::setw(20) << std::fixed
			<< std::setprecision(2) << C_BLURPLE
			<< _GetRateForDate(date).second * value << C_RESET << std::endl;
}

ExchangePair	BitcoinExchange::_GetRateForDate(
	BitcoinExchange::Date_t date
)
const throw(ExchangeException)
{
	ExchangePair	nearestRate;

	for (const_exchange_iterator	it = _map.begin(); it != _map.end(); it++)
	{
		const std::string	key = it->first;
		const double		rate = it->second;
		Date_t				itDate = _IsValidDateFormat(key);

		if (itDate <= date)
		{
			nearestRate = ExchangePair(key, rate);
			continue;
		}
		if (_map.begin() == it)
			throw ExchangeException("The date is too old!");

		break;
	}

	return nearestRate;
}

void	BitcoinExchange::ParseDatabase(void) throw(ExchangeException)
{
	std::cout << C_RESET C_CLEAR C_HOME << std::endl;
	_db.open(_dbFilename);

	if (!_db.is_open())
		throw ExchangeException("The database file could not be opened.");
	
	std::string	line;
	int64_t		i = -1;
	while (std::getline(_db, line) && (++i + 1))
	{
		if (i == 0)
		{
			if (line != "date,exchange_rate") throw ExchangeException(
				"Invalid data header."
			);
			continue;
		}
		try
		{
			_SaveLine(line);
		}
		catch (const ExchangeException & e)
		{
			_db.close();
			throw;
		}
	}
	_db.close();
	std::cout << C_GREEN "Database successfully registered into the memory."
				C_RESET << std::endl;
}

void	BitcoinExchange::ReadInput(void) throw(ExchangeException)
{
	std::cout << C_RESET C_CLEAR C_HOME << std::endl;
	_input.open(_inputFilename);

	if (!_input.is_open())
		throw ExchangeException("The input file could not be opened.");
	
	std::string	line;
	int64_t		i = -1;
	while (std::getline(_input, line) && (++i + 1))
	{
		if (i == 0)
		{
			if (line != "date | value") throw ExchangeException(
				"Invalid input header."
			);
			continue;
		}
		try
		{
			_UnderstandLine(line);
		}
		catch (const ExchangeException & e)
		{
			std::cout << e.what() << "\n" << std::endl;
		}
	}
	_input.close();
	std::cout << C_GREEN "Input successfully read." C_RESET << std::endl;
}
