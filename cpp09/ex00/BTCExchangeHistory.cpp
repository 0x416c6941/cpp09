#include "BTCExchangeHistory.hpp"
#include <string>
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

void BTCExchangeHistory::read_history(std::ifstream & file)
{
	bool got_at_least_first_line = false;
	enum e_first_db_column first_column;
	std::string line;
	const std::string MSG_PREFIX = "BTCExchangeHistory::read_history(): ";
	const std::string EMPTY_FILE_MSG = "File is empty.";
	const std::string FILE_CORRUPTION_MSG = "File is corrupted.";

	while (std::getline(file, line))
	{
		if (file.eof())
		{
			if (!got_at_least_first_line)
			{
				throw HistoryFileIsCorrupted(MSG_PREFIX
						+ EMPTY_FILE_MSG);
			}
			break;
		}
		else if (file.fail())
		{
			throw HistoryFileIsCorrupted(MSG_PREFIX
					+ FILE_CORRUPTION_MSG);
		}
		if (!got_at_least_first_line)
		{
			this->handle_first_line(line, first_column);
			got_at_least_first_line = true;
			// TODO: remove this.
			break;
		}
	}
}
