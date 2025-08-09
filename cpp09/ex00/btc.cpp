#include "btc.hpp"
#include "BTCExchangeHistory.hpp"
#include <fstream>
#include <string>
#include "Date.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cfloat>

// It'd be great to split this function into smaller sub-functions.
// However, it's the finish line and I'm too lazy to do that. Oopsie.
int convert(const BTCExchangeHistory & exchange_history, std::ifstream & file)
{
	std::string line;
	Date * date = NULL;
	double val;
	const char * line_c_str_begin;
	char * line_c_str_pos;
	size_t processed_bytes;
	// Range of possible BTC amount: [0, MAX].
	const double MAX = 1000.0;
	const std::string FIRST_LINE_FORMAT = "date | value";
	const std::string COLUMN_SEP = " | ";
	const std::string IO_FAIL_MSG = "Error: I/O fail during file reading.";
	const std::string NEGATIVE_NUM_MSG = "Error: not a positive number.";
	const std::string BAD_INPUT_MSG = "Error: bad input => ";
	const std::string TOO_LARGE_NUM_MSG = "Error: too large a number.";
	const std::string NO_LOWER_DATE_MSG = "Error: No closest lower date is available.";

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
		line_c_str_begin = line.c_str() + processed_bytes;
		errno = 0;
		val = strtod(line_c_str_begin, &line_c_str_pos);
		if (*line_c_str_pos != '\0' || errno == ERANGE)
		{
			std::cerr << BAD_INPUT_MSG << line << std::endl;
			delete date;
			continue;
		}
		else if (val < 0)
		{
			std::cerr << NEGATIVE_NUM_MSG << std::endl;
			delete date;
			continue;
		}
		else if (val >= MAX
			&& !(std::fabs(val - MAX) < DBL_EPSILON))
		{
			std::cerr << TOO_LARGE_NUM_MSG << std::endl;
			delete date;
			continue;
		}
		try
		{
			// With the given example in subject,
			// we don't need to handle precision or whatever else.
			std::cout << *date
					<< " => " << val
					<< " = " << val * exchange_history.
						get_btc_exchange_rate(*date)
					<< std::endl;
		}
		catch (const BTCExchangeHistory::NoAvailableLowerDate & e)
		{
			std::cerr << NO_LOWER_DATE_MSG << std::endl;
			delete date;
			continue;
		}
		delete date;
	}
	return 0;
}
