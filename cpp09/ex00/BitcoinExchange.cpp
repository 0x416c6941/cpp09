#include "BitcoinExchange.hpp"
#include <string>
#include <cstddef>
#include <sstream>
#include <cerrno>
#include <cstdlib>
#include <iomanip>
#include <iterator>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange & src)
	: history(src.history)
{
}

BitcoinExchange & BitcoinExchange::operator = (const BitcoinExchange & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->history = src.history;
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange::HistoryFileIsCorrupted::HistoryFileIsCorrupted(
		const char * msg)
	: msg(msg)
{
}

BitcoinExchange::HistoryFileIsCorrupted::HistoryFileIsCorrupted(
		const std::string & msg)
	: msg(msg)
{
}

BitcoinExchange::HistoryFileIsCorrupted::HistoryFileIsCorrupted(
		const BitcoinExchange::HistoryFileIsCorrupted & src)
	: msg(src.msg)
{
}

BitcoinExchange::HistoryFileIsCorrupted & BitcoinExchange::HistoryFileIsCorrupted::operator = (
		const BitcoinExchange::HistoryFileIsCorrupted & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

BitcoinExchange::HistoryFileIsCorrupted::~HistoryFileIsCorrupted() throw()
{
}

const char * BitcoinExchange::HistoryFileIsCorrupted::what() const throw()
{
	return this->msg.c_str();
}

BitcoinExchange::InvalidDate::InvalidDate(const char * msg)
	: msg(msg)
{
}

BitcoinExchange::InvalidDate::InvalidDate(const std::string & msg)
	: msg(msg)
{
}

BitcoinExchange::InvalidDate::InvalidDate(
		const BitcoinExchange::InvalidDate & src)
	: msg(src.msg)
{
}

BitcoinExchange::InvalidDate & BitcoinExchange::InvalidDate::operator = (
		const BitcoinExchange::InvalidDate & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

BitcoinExchange::InvalidDate::~InvalidDate() throw()
{
}

const char * BitcoinExchange::InvalidDate::what() const throw()
{
	return this->msg.c_str();
}

BitcoinExchange::NoAvailableLowerDate::NoAvailableLowerDate(const char * msg)
	: msg(msg)
{
}

BitcoinExchange::NoAvailableLowerDate::NoAvailableLowerDate(const std::string & msg)
	: msg(msg)
{
}

BitcoinExchange::NoAvailableLowerDate::NoAvailableLowerDate(
		const BitcoinExchange::NoAvailableLowerDate & src)
	: msg(src.msg)
{
}

BitcoinExchange::NoAvailableLowerDate & BitcoinExchange::NoAvailableLowerDate::operator = (
		const BitcoinExchange::NoAvailableLowerDate & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

BitcoinExchange::NoAvailableLowerDate::~NoAvailableLowerDate() throw()
{
}

const char * BitcoinExchange::NoAvailableLowerDate::what() const throw()
{
	return this->msg.c_str();
}

void BitcoinExchange::handle_first_line(const std::string & line,
		enum e_first_db_column & first_column)
{
	const std::string DATE = "date", EXCHANGE_RATE = "exchange_rate";
	size_t processed_bytes = 0;
	const std::string MSG_PREFIX = "BitcoinExchange::handle_first_line(): ";

	// First column name.
	if (line.compare(processed_bytes, DATE.length(), DATE) == 0)
	{
		first_column = BitcoinExchange::DATE;
		processed_bytes += DATE.length();
	}
	else if (line.compare(processed_bytes,
			EXCHANGE_RATE.length(), EXCHANGE_RATE) == 0)
	{
		first_column = BitcoinExchange::EXCHANGE_RATE;
		processed_bytes += EXCHANGE_RATE.length();
	}
	else
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "Incorrect name of first column.");
	}
	// ',' after first column name.
	if (!(processed_bytes < line.length())
		|| line.at(processed_bytes) != ',')
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "Didn't receive ',' after first column name.");
	}
	processed_bytes++;	// Skipping ','.
	// Second column name.
	if (first_column == BitcoinExchange::DATE)
	{
		if (line.compare(processed_bytes,
				EXCHANGE_RATE.length(), EXCHANGE_RATE) != 0)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ "Incorrect name of second column.");
		}
		processed_bytes += EXCHANGE_RATE.length();
	}
	else if (first_column == BitcoinExchange::EXCHANGE_RATE)
	{
		if (line.compare(processed_bytes,
				DATE.length(), DATE) != 0)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ "Incorrect name of second column.");
		}
		processed_bytes += DATE.length();
	}
	// After second column name, first line should end.
	if (line.length() != processed_bytes)
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "First line didn't end after second column name.");
	}
}

// This method contains duplicate code.
// Too lazy to split it, sorry.
void BitcoinExchange::append_history(const std::string & line,
		const enum e_first_db_column & first_column)
{
	std::string date;
	double val;
	size_t processed_bytes = 0;
	const char * line_c_str_begin;
	char * line_c_str_pos;
	const std::string MSG_PREFIX = "BitcoinExchange::append_history(): ";
	const std::string INVALID_DATE_MSG = "Some date is invalid.";
	const std::string INVALID_RATE_MSG = "Some exchange rate is invalid.";

	// First column.
	if (first_column == DATE)
	{
		try
		{
			date = get_date_substr(line, processed_bytes);
		}
		catch (const InvalidDate & e)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ INVALID_DATE_MSG);
		}
	}
	else if (first_column == EXCHANGE_RATE)
	{
		line_c_str_begin = line.c_str() + processed_bytes;
		errno = 0;
		val = strtod(line_c_str_begin, &line_c_str_pos);
		// We don't need DBL_EPSILON to compare with range.
		// DBL_EPSILON is only used when comparing
		// with a specific value.
		if (errno == ERANGE || val < 0)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ INVALID_RATE_MSG);
		}
		processed_bytes += static_cast<size_t> (line_c_str_pos
				- line_c_str_begin);
	}
	// ',' after first column.
	if (!(processed_bytes < line.length())
		|| line.at(processed_bytes) != ',')
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "No comma separator between columns.");
	}
	processed_bytes++;	// Skipping ','.
	// Second column.
	if (first_column == DATE)
	{
		line_c_str_begin = line.c_str() + processed_bytes;
		errno = 0;
		val = strtod(line_c_str_begin, &line_c_str_pos);
		// We don't need DBL_EPSILON to compare with range.
		// DBL_EPSILON is only used when comparing
		// with a specific value.
		if (errno == ERANGE || val < 0)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ INVALID_RATE_MSG);
		}
		processed_bytes += static_cast<size_t> (line_c_str_pos
				- line_c_str_begin);
	}
	else if (first_column == EXCHANGE_RATE)
	{
		size_t date_substr_len_in_line;
		try
		{
			date = get_date_substr(line.substr(processed_bytes),
					date_substr_len_in_line);
		}
		catch (const InvalidDate & e)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ INVALID_DATE_MSG);
		}
		processed_bytes += date_substr_len_in_line;
	}
	// Checking if line ends after second column.
	if (line.length() != processed_bytes)
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "Got data after second column.");
	}
	else if (this->history.find(date) != this->history.end())
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "Some date is present multiple times.");
	}
	// Appending to `this->history`.
	this->history[date] = val;
}

std::string BitcoinExchange::get_date_substr(const std::string & line,
		size_t & processed_bytes_in_line) const
{
	std::stringstream ret;
	long year, month, day;
	const char * line_c_str_begin = line.c_str();
	char * line_c_str_pos;
	const int STRTOL_BASE = 10, MM_DD_WIDTH = 2;
	const std::string EXCEPTION_MSG = std::string("BitcoinExchange::get_date(): ")
			+ "Date stored in line is invalid.";

	errno = 0;
	year = strtol(line_c_str_begin, &line_c_str_pos, STRTOL_BASE);
	if (*line_c_str_pos++ != '-' || errno == ERANGE)
	{
		throw InvalidDate(EXCEPTION_MSG);
	}
	month = strtol(line_c_str_pos, &line_c_str_pos, STRTOL_BASE);
	if (*line_c_str_pos++ != '-' || errno == ERANGE
		|| month < 1 || month > 12)
	{
		throw InvalidDate(EXCEPTION_MSG);
	}
	day = strtol(line_c_str_pos, &line_c_str_pos, STRTOL_BASE);
	// Basic day range check.
	if (errno == ERANGE
		|| day < 1 || day > 31)
	{
		throw InvalidDate(EXCEPTION_MSG);
	}
	// Day range check for months w/ only 30 days.
	else if ((month == 4 || month == 6 || month == 9 || month == 11)
		&& day > 30)
	{
		throw InvalidDate(EXCEPTION_MSG);
	}
	// Day range check for February and leap year.
	else if (month == 2)
	{
		// Leap year.
		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)
			&& day > 29)
		{
			throw InvalidDate(EXCEPTION_MSG);
		}
		// Non-leap year.
		else if (day > 28)
		{
			throw InvalidDate(EXCEPTION_MSG);
		}
	}
	processed_bytes_in_line = static_cast<size_t> (line_c_str_pos
			- line_c_str_begin);
	ret << year
		<< '-' << std::setw(MM_DD_WIDTH) << std::setfill('0') << month
		<< '-' << std::setw(MM_DD_WIDTH) << std::setfill('0') << day;
	return ret.str();
}

void BitcoinExchange::read_history(std::ifstream & file)
{
	bool got_first_line = false;
	enum e_first_db_column first_column;
	std::string line;
	const std::string MSG_PREFIX = "BitcoinExchange::read_history(): ";
	const std::string FILE_CORRUPTION_MSG = "File is corrupted.";

	history.clear();
	// Any empty lines should be considered invalid,
	// since we're working w/ a "database" here.
	while (std::getline(file, line))
	{
		if (file.eof())
		{
			break;
		}
		else if (file.fail())
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ FILE_CORRUPTION_MSG);
		}
		if (!got_first_line)
		{
			this->handle_first_line(line, first_column);
			got_first_line = true;
		}
		else
		{
			this->append_history(line, first_column);
		}
	}
	if (!got_first_line)
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "DB file is completely empty.");
	}
	// We consider database with empty table a valid database.
}

double BitcoinExchange::get_btc_exchange_rate(const std::string & date) const
{
	std::map<std::string, double>::const_iterator it;
	const std::string MSG_PREFIX = "BitcoinExchange::get_btc_exchange_rate(): ";

	// "... first element that is not less than the given key ..."
	it = this->history.lower_bound(date);
	if (it == this->history.end())
	{
		if (this->history.empty())
		{
			throw NoAvailableLowerDate(MSG_PREFIX
					+ "History is empty.");
		}
		it = this->history.end();
		std::advance(it, -1);
		return it->second;
	}
	else if (date < it->first)
	{
		// It's really unclear what we should do in this case.
		// Subject specifies only this:
		// "Be careful to use the lower date and not the upper one",
		// and in their example there's a date "2011-01-09",
		// which is clearly closer to "2011-01-10"
		// rather than "2011-01-07", yet they still used "2011-01-07".
		// So, if it means we shall never use the upper date,
		// we should clearly throw an exception here.
		if (it == this->history.begin())
		{
			throw NoAvailableLowerDate(MSG_PREFIX
					+ "No lower date is available in history.");
		}
		std::advance(it, -1);
		return it->second;
	}
	return it->second;
}
