#include "btc.hpp"
#include "BTCExchangeHistory.hpp"
#include <fstream>
#include <string>
#include "Date.hpp"
#include <iostream>
#include <cstdlib>

int convert(const BTCExchangeHistory & exchange_history, std::ifstream & file)
{
	std::string line;
	Date * date = NULL;
	double val;
	size_t processed_bytes;
	const std::string FIRST_LINE_FORMAT = "date | value";
	const std::string COLUMN_SEP = " | ";
	const std::string IO_FAIL_MSG = "Error: I/O fail during file reading.";
	const std::string BAD_INPUT_MSG = "Error: bad input => ";

	std::getline(file, line);
	if (file.eof())
	{
		std::cerr << "Error: File is empty." << std::endl;
		return EXIT_FAILURE;
	}
	else if (file.fail())
	{
		std::cerr << IO_FAIL_MSG << std::endl;
		return EXIT_FAILURE;
	}
	else if (line != FIRST_LINE_FORMAT)
	{
		std::cerr << "Error: File isn't an expected database."
				<< std::endl;
		return EXIT_FAILURE;
	}
	// Any empty lines will be considered invalid,
	// since we're working w/ a "database" here.
	while (std::getline(file, line))
	{
		if (file.eof())
		{
			break;
		}
		else if (file.fail())
		{
			std::cerr << IO_FAIL_MSG << std::endl;
			return EXIT_FAILURE;
		}
		// Reading date.
		try
		{
			date = new Date(line);
		}
		catch (const Date::InvalidDate & e)
		{
			std::cerr << BAD_INPUT_MSG << line << std::endl;
			continue;
		}
		processed_bytes = date->get_processed_bytes_in_date_string();
		// Separator between date and value.
		if (line.compare(processed_bytes,
				COLUMN_SEP.length(), COLUMN_SEP) != 0
			|| !(processed_bytes + COLUMN_SEP.length()
				< line.length()))
		{
			std::cerr << BAD_INPUT_MSG << line << std::endl;
			delete date;
			continue;
		}
		processed_bytes += COLUMN_SEP.length();	// Skipping separator.
		(void) val;
		(void) exchange_history;
		delete date;
	}
	return 0;
}
