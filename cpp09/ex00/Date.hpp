#ifndef DATE_HPP
#define DATE_HPP

#include <string>
#include <stdexcept>

/**
 * A class to parse the date stored as string in "YYYY-MM-DD" format.
 * @class	Date
 */
class Date
{
	private:
		// Storing as long is easiest due to usage of `strtol()`.
		long year, month, day;

		// How many bytes in `date` were processed
		// in `Date::Date(const std::string &)'.
		size_t processed_bytes_in_date_string;

		// We don't want the default constructor to be available.
		Date();

	public:
		/**
		 * Constructor for the "Date" object
		 * with \p date in "YYYY-MM-DD" format.
		 * @param	date	Date stored as a string
		 * 			in "YYYY-MM-DD" format.
		 * @throw	invalid_date	\p date is in invalid format.
		 */
		Date(const std::string & date);
		Date(const Date & src);
		Date & operator = (const Date & src);
		virtual ~Date();

		bool operator < (const Date & other) const;

		/**
		 * An exception that's thrown when
		 * `Date::Date(const std::string &)`
		 * encounters invalid argument.
		 * @class	invalid_date
		 */
		class invalid_date: public std::exception
		{
			private:
				std::string msg;

			public:
				invalid_date(const char * msg);
				invalid_date(const std::string & msg);
				invalid_date(const invalid_date & src);
				invalid_date & operator = (const invalid_date & src);
				virtual ~invalid_date() throw();

				virtual const char * what() const throw();
		};

		long get_year() const;
		long get_month() const;
		long get_day() const;
		size_t get_processed_bytes_in_date_string() const;
};

#endif	// DATE_HPP
