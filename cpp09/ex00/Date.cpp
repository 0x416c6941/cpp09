#include "Date.hpp"
#include <string>
#include <cerrno>
#include <cstdlib>
#include <limits>

Date::Date(const std::string & date)
{
	const char * date_c_str_begin = date.c_str();
	char * date_c_str_pos;
	const int STRTOL_BASE = 10;
	// Year must be at least 2 AD, since Howard Hinnant's formula
	// works only after March, 1 AD.
	const long MIN_YEAR = 2;
	const std::string EXCEPTION_MSG = std::string("Date::Date(): ")
			+ "Date string is invalid.";

	errno = 0;
	this->year = strtol(date_c_str_begin, &date_c_str_pos, STRTOL_BASE);
	if (*date_c_str_pos++ != '-' || errno == ERANGE
		|| this->year < MIN_YEAR || this->year > this->MAX_POSSIBLE_YEAR)
	{
		throw InvalidDate(EXCEPTION_MSG);
	}
	this->month = strtol(date_c_str_pos, &date_c_str_pos, STRTOL_BASE);
	if (*date_c_str_pos++ != '-' || errno == ERANGE
		|| this->month < 1 || this->month > 12)
	{
		throw InvalidDate(EXCEPTION_MSG);
	}
	this->day = strtol(date_c_str_pos, &date_c_str_pos, STRTOL_BASE);
	// Basic day range check.
	if (errno == ERANGE
		|| this->day < 1 || this->day > 31)
	{
		throw InvalidDate(EXCEPTION_MSG);
	}
	// Day range check for months w/ only 30 days.
	else if ((this->month == 4 || this->month == 6
			|| this->month == 9 || this->month == 11)
		&& day > 30)
	{
		throw InvalidDate(EXCEPTION_MSG);
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
				throw InvalidDate(EXCEPTION_MSG);
			}
		}
		else if (this->day > 28)
		{
			throw InvalidDate(EXCEPTION_MSG);
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

Date::InvalidDate::InvalidDate(const char * msg)
	: msg(msg)
{
}

Date::InvalidDate::InvalidDate(const std::string & msg)
	: msg(msg)
{
}

Date::InvalidDate::InvalidDate(const Date::InvalidDate & src)
	: msg(src.msg)
{
}

Date::InvalidDate & Date::InvalidDate::operator = (const Date::InvalidDate & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

Date::InvalidDate::~InvalidDate() throw()
{
}

const char * Date::InvalidDate::what() const throw()
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

long Date::get_reference_point_for_date_comparison() const
{
	const long Y = this->year;

	// It's guaranteed that nothing would overflow
	// thanks to the maximum possible year cap
	// with `this->MAX_POSSIBLE_YEAR`.
	return 365L * Y + Y / 4 - Y / 100 + Y / 400
		+ (153 * this->month - 457) / 5 + this->day - 306;
}

long operator - (const Date & lhs, const Date & rhs)
{
	return lhs.get_reference_point_for_date_comparison()
		- rhs.get_reference_point_for_date_comparison();
}
