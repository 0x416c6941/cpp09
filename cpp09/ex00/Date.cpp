#include "Date.hpp"
#include <string>
#include <cerrno>
#include <cstdlib>
#include <limits>

Date::Date(const std::string & date)
{
	const char * date_c_str_begin = date.c_str();
	char * date_c_str_pos = (char *) date.c_str();
	const int STRTOL_BASE = 10;
	const std::string EXCEPTION_MSG = std::string("Date::Date(const std::string &): ")
			+ "Date string is invalid.";

	errno = 0;
	this->year = strtol(date_c_str_pos, &date_c_str_pos, STRTOL_BASE);
	if (*date_c_str_pos++ != '-' || errno == ERANGE
		|| this->year < 1)
	{
		throw invalid_date(EXCEPTION_MSG);
	}
	this->month = strtol(date_c_str_pos, &date_c_str_pos, STRTOL_BASE);
	if (*date_c_str_pos++ != '-' || errno == ERANGE
		|| this->month < 1 || this->month > 12)
	{
		throw invalid_date(EXCEPTION_MSG);
	}
	this->day = strtol(date_c_str_pos, &date_c_str_pos, STRTOL_BASE);
	// Basic day range check.
	if ((*date_c_str_pos != ',' && *date_c_str_pos != '\0'
			&& *date_c_str_pos != ' ') || errno == ERANGE
		|| this->day < 1 || this->day > 31)
	{
		throw invalid_date(EXCEPTION_MSG);
	}
	// Day range check for months w/ only 30 days.
	else if ((this->month == 4 || this->month == 6
			|| this->month == 9 || this->month == 11)
		&& day > 30)
	{
		throw invalid_date(EXCEPTION_MSG);
	}
	// Day range check for February and leap year.
	else if (this->month == 2)
	{
		// Leap year.
		if (this->year % 4 == 0
			&& (this->year % 100 != 0 || this->year % 400 == 0))
		{
			if (this->day > 29)
			{
				throw invalid_date(EXCEPTION_MSG);
			}
		}
		else if (this->day > 28)
		{
			throw invalid_date(EXCEPTION_MSG);
		}
	}
	this->processed_bytes_in_date_string = static_cast<size_t>
			(date_c_str_pos - date_c_str_begin);
}

Date::Date(const Date & src)
	:	year(src.year), month(src.month), day(src.day),
		processed_bytes_in_date_string(src.processed_bytes_in_date_string)
{
}

Date & Date::operator = (const Date & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->year = src.year;
	this->month = src.month;
	this->day = src.day;
	this->processed_bytes_in_date_string = src.processed_bytes_in_date_string;
	return *this;
}

Date::~Date()
{
}

bool Date::operator < (const Date & other) const
{
	// Year comparison.
	if (this->year < other.year)
	{
		return true;
	}
	else if (this->year > other.year)
	{
		return false;
	}
	// Month comparison.
	if (this->month < other.month)
	{
		return true;
	}
	else if (this->month > other.month)
	{
		return false;
	}
	// Day comparison.
	if (this->day < other.day)
	{
		return true;
	}
	return false;	// Day is same or bigger.
}

Date::invalid_date::invalid_date(const char * msg)
	: msg(msg)
{
}

Date::invalid_date::invalid_date(const std::string & msg)
	: msg(msg)
{
}

Date::invalid_date::invalid_date(const Date::invalid_date & src)
	: msg(src.msg)
{
}

Date::invalid_date & Date::invalid_date::operator = (const Date::invalid_date & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

Date::invalid_date::~invalid_date() throw()
{
}

const char * Date::invalid_date::what() const throw()
{
	return msg.c_str();
}

long Date::get_year() const
{
	return this->year;
}

long Date::get_month() const
{
	return this->month;
}

long Date::get_day() const
{
	return this->day;
}

size_t Date::get_processed_bytes_in_date_string() const
{
	return this->processed_bytes_in_date_string;
}
