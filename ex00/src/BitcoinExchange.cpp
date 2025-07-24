#include "BitcoinExchange.hpp"

std::string	BitcoinExchange::dbFilenameDefault = "data.csv";

BitcoinExchange::BitcoinExchange(void):
	_dbFilename(dbFilenameDefault), _inputFilename("input.csv")
{}

BitcoinExchange::BitcoinExchange(const std::string & inputFilename):
	_dbFilename(dbFilenameDefault), _inputFilename(inputFilename)
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange & other):
	_dbFilename(other._dbFilename),
	_inputFilename(other._inputFilename),
	_map(other._map)
{}

BitcoinExchange::~BitcoinExchange(void)
{
	_map.clear();
}

BitcoinExchange & BitcoinExchange::operator=(const BitcoinExchange & other)
{
	if (this != &other)
	{
		this->_dbFilename = other._dbFilename;
		this->_inputFilename = other._inputFilename;
		this->_map = other._map;
	}

	return *this;
}
