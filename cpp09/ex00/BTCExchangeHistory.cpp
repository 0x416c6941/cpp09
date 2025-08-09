#include "BTCExchangeHistory.hpp"
#include <string>
#include "Date.hpp"
#include <cerrno>
#include <cstdlib>
#include <fstream>

BTCExchangeHistory::BTCExchangeHistory()
{
}

BTCExchangeHistory::BTCExchangeHistory(const BTCExchangeHistory & src)
	: history(src.history)
{
}

BTCExchangeHistory & BTCExchangeHistory::operator = (
		const BTCExchangeHistory & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->history = src.history;
	return *this;
}

BTCExchangeHistory::~BTCExchangeHistory()
{
}

BTCExchangeHistory::HistoryFileIsCorrupted::HistoryFileIsCorrupted(
		const char * msg)
	: msg(msg)
{
}

BTCExchangeHistory::HistoryFileIsCorrupted::HistoryFileIsCorrupted(
		const std::string & msg)
	: msg(msg)
{
}

BTCExchangeHistory::HistoryFileIsCorrupted::HistoryFileIsCorrupted(
		const BTCExchangeHistory::HistoryFileIsCorrupted & src)
	: msg(src.msg)
{
}

BTCExchangeHistory::HistoryFileIsCorrupted & BTCExchangeHistory::HistoryFileIsCorrupted::operator = (
		const BTCExchangeHistory::HistoryFileIsCorrupted & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

BTCExchangeHistory::HistoryFileIsCorrupted::~HistoryFileIsCorrupted() throw()
{
}

const char * BTCExchangeHistory::HistoryFileIsCorrupted::what() const throw()
{
	return msg.c_str();
}

void BTCExchangeHistory::handle_first_line(const std::string & line,
		enum e_first_db_column & first_column)
{
	const std::string DATE = "date", EXCHANGE_RATE = "exchange_rate";
	size_t processed_bytes = 0;
	const std::string MSG_PREFIX = "BTCExchangeHistory::handle_first_line(): ";

	// First column name.
	if (line.compare(processed_bytes, DATE.length(), DATE) == 0)
	{
		first_column = BTCExchangeHistory::DATE;
		processed_bytes += DATE.length();
	}
	else if (line.compare(processed_bytes,
			EXCHANGE_RATE.length(), EXCHANGE_RATE) == 0)
	{
		first_column = BTCExchangeHistory::EXCHANGE_RATE;
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
	if (first_column == BTCExchangeHistory::DATE)
	{
		if (line.compare(processed_bytes,
				EXCHANGE_RATE.length(), EXCHANGE_RATE) != 0)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ "Incorrect name of second column.");
		}
		processed_bytes += EXCHANGE_RATE.length();
	}
	else if (first_column == BTCExchangeHistory::EXCHANGE_RATE)
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
void BTCExchangeHistory::append_history(const std::string & line,
		const enum e_first_db_column & first_column)
{
	const char * line_c_str_begin;
	char * line_c_str_pos;
	Date * date = NULL;
	double val;
	size_t processed_bytes = 0;
	const std::string MSG_PREFIX = "BTCExchangeRate::append_history(): ";
	const std::string INVALID_DATE_MSG = "Some date is invalid.";
	const std::string INVALID_RATE_MSG = "Some exchange rate is invalid.";

	// First column.
	if (first_column == DATE)
	{
		try
		{
			date = new Date(line.substr(processed_bytes));
		}
		catch (const Date::InvalidDate & e)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ INVALID_DATE_MSG);
		}
		processed_bytes += date->get_processed_bytes_in_date_string();
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
		processed_bytes += static_cast<size_t> (
				line_c_str_pos - line_c_str_begin);
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
			delete date;
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ INVALID_RATE_MSG);
		}
		processed_bytes += static_cast<size_t> (
				line_c_str_pos - line_c_str_begin);
	}
	else if (first_column == EXCHANGE_RATE)
	{
		try
		{
			date = new Date(line.substr(processed_bytes));
		}
		catch (const Date::InvalidDate & e)
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ INVALID_DATE_MSG);
		}
		processed_bytes += date->get_processed_bytes_in_date_string();
	}
	// Checking if line ends after second column.
	if (line.length() != processed_bytes)
	{
		delete date;
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "Got data after second column.");
	}
	else if (this->history.find(*date) != this->history.end())
	{
		delete date;
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "Some date is present multiple times.");
	}
	// Appending to `this->history`.
	this->history[*date] = val;
	delete date;
}

void BTCExchangeHistory::read_history(std::ifstream & file)
{
	bool got_first_line = false;
	enum e_first_db_column first_column;
	std::string line;
	const std::string MSG_PREFIX = "BTCExchangeHistory::read_history(): ";
	const std::string FILE_CORRUPTION_MSG = "File is corrupted.";

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
	// I believe the database should be considered valid
	// if there's at least one entry,
	// not just the first line w/ column order.
	if (this->history.empty())
	{
		throw HistoryFileIsCorrupted(MSG_PREFIX
				+ "DB didn't contain a single entry.");
	}
}
