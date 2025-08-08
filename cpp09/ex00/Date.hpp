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
		// We have neither `cstdint` nor `long long` available in C++98.
		// Therefore, we need to cap the maximum possible year
		// to prevent any overflows in
		// `get_reference_point_for_date_comparison()`.
		const static long MAX_POSSIBLE_YEAR = 9999;

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

		// For `std::map`, where `Date` is key,
		// and for `std::map<Date, ...>::{lower_bound,upper_bound}'.
		bool operator < (const Date & other) const;

		/**
		 * An exception that's thrown when
		 * `Date::Date(const std::string &)`
		 * encounters invalid argument.
		 * @class	invalid_date
		 */
		class InvalidDate: public std::exception
		{
			private:
				std::string msg;

			public:
				InvalidDate(const char * msg);
				InvalidDate(const std::string & msg);
				InvalidDate(const InvalidDate & src);
				InvalidDate & operator = (const InvalidDate & src);
				virtual ~InvalidDate() throw();

				virtual const char * what() const throw();
		};

		long get_year() const;
		long get_month() const;
		long get_day() const;
		size_t get_processed_bytes_in_date_string() const;

		/**
		 * Get the reference point to calculate the difference
		 * between two dates by using Howard Hinnant's formula.
		 * @warning	Epoch is not 1st of January, 1 AD,
		 * 		but some arbitrary, yet consistent,
		 * 		reference point.
		 * @return	Reference point for two dates' comparison.
		 */
		long get_reference_point_for_date_comparison() const;
};

/**
 * Return difference in days between \p lhs and \p rhs.
 * @param	lhs	First date.
 * @param	rhs	Second date.
 * @return	Difference in days between \p lhs and \p rhs.
 */
long operator - (const Date & lhs, const Date & rhs);

#endif	// DATE_HPP
