#include "BitcoinExchange.hpp"
#include <fstream>
#include <string>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <stdexcept>

/**
 * Read the input \p file, check the first line,
 * and convert all bitcoins to currency according to exchange rates
 * stored in \p exchange_history.
 * @param	exchange_history	BTC exchange rates per days.
 * @param	file			Input database, where the first line
 * 					is "date | value",
 * 					and all later entries
 * 					are in this format.
 * @return	Program's exit status:
 * 		@c EXIT_FAILURE, if got I/O error while reading \p file,
 * 			or if \p file didn't contain
 * 			even a first "date | value" line.
 * 		0 in all other cases.
 */
static int convert(const BitcoinExchange & btc_exchange,
		std::ifstream & file)
{
	std::string date;
	double val;
	size_t processed_bytes_in_line;
	std::string line;
	// Range of possible BTC amount: [0, MAX].
	const double MAX = 1000.0;
	const char * line_c_str_begin;
	char * line_c_str_pos;
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
		processed_bytes_in_line = 0;
		// Reading date.
		try
		{
			date = btc_exchange.get_date_substr(line,
					processed_bytes_in_line);
		}
		catch (const BitcoinExchange::InvalidDate & e)
		{
			std::cerr << BAD_INPUT_MSG << line << std::endl;
			continue;
		}
		// Separator between date and value.
		if (line.compare(processed_bytes_in_line,
				COLUMN_SEP.length(), COLUMN_SEP) != 0
			|| !(processed_bytes_in_line + COLUMN_SEP.length()
				< line.length()))
		{
			std::cerr << BAD_INPUT_MSG << line << std::endl;
			continue;
		}
		processed_bytes_in_line += COLUMN_SEP.length();	// Skipping separator.
		// Reading value.
		line_c_str_begin = line.c_str() + processed_bytes_in_line;
		errno = 0;
		val = strtod(line_c_str_begin, &line_c_str_pos);
		if (*line_c_str_pos != '\0' || errno == ERANGE)
		{
			std::cerr << BAD_INPUT_MSG << line << std::endl;
			continue;
		}
		else if (val < 0)
		{
			std::cerr << NEGATIVE_NUM_MSG << std::endl;
			continue;
		}
		else if (val >= MAX
			&& !(std::fabs(val - MAX) < DBL_EPSILON))
		{
			std::cerr << TOO_LARGE_NUM_MSG << std::endl;
			continue;
		}
		// Convertion and output.
		try
		{
			// With the given example in subject,
			// we don't need to handle precision or whatever else.
			std::cout << date
					<< " => " << val
					<< " = " << val
						* btc_exchange.get_btc_exchange_rate(
							date)
					<< std::endl;
		}
		catch (const BitcoinExchange::NoAvailableLowerDate & e)
		{
			std::cerr << NO_LOWER_DATE_MSG << std::endl;
			continue;
		}
	}
	return 0;
}

// Ideally, to prevent collision with already existing projects,
// we should've been defined a namespace for our functions and classes.
// Still, at this point everything works and I don't really care so much.
int main(int argc, char * argv[])
{
	const int EXPECTED_ARGS = 2;
	const std::string INPUT_FILE_OPEN_FAIL = "Error: could not open file.";
	const std::string DB_FILE = "data.csv";
	const int FILE_PATH = 1;
	std::ifstream file;
	BitcoinExchange btc_exchange;

	if (argc != EXPECTED_ARGS)
	{
		std::cerr << INPUT_FILE_OPEN_FAIL << std::endl;
		return EXIT_FAILURE;
	}
	file.open(DB_FILE.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: Couldn't open the DB file "
				<< DB_FILE << '.' << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		btc_exchange.read_history(file);
	}
	catch (const BitcoinExchange::HistoryFileIsCorrupted & e)
	{
		std::cerr << "Error: DB file "
				<< DB_FILE << " is corrupted." << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << "Error: OOM." << std::endl;
		return EXIT_FAILURE;
	}
	file.close();
	file.open(argv[FILE_PATH]);
	if (!file.is_open())
	{
		std::cerr << INPUT_FILE_OPEN_FAIL << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		return convert(btc_exchange, file);
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << "Error: OOM." << std::endl;
		return EXIT_FAILURE;
	}
}
