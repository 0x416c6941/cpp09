#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <cstddef>
#include <fstream>
#include <stdexcept>

/**
 * A class to parse and store BTC exchange rate across multiple years
 * and exchange some given BTC amount on a given date.
 * @class	BitcoinExchange
 */
class BitcoinExchange
{
	private:
		std::map<std::string, double> history;

		// I really don't want to solve the date
		// with std::setw() and std::setfill().
		static const long MIN_YEAR = 1000, MAX_YEAR = 9999;

		// For internal use in `read_history()`.
		// It's not stated in subject that first column
		// must necessarily be "date", it may therefore
		// theoretically also be "exchange_rate".
		enum e_first_db_column
		{
			DATE,
			EXCHANGE_RATE
		};

		/**
		 * Take care of first \p line of BTC exchange rate history.
		 * @throw	HistoryFileIsCorrupted	First \p line
		 * 					is corrupted.
		 * @param	line		Line from file with
		 * 				BTC exchange rate history.
		 * @param	first_column	Where to store info about
		 * 				which column in the DB
		 * 				is first.
		 */
		void handle_first_line(const std::string & line,
				enum e_first_db_column & first_column);

		/**
		 * Append entry from \p line to `history`.
		 * @throw	HistoryFileIsCorrupted	\p line is corrupted.
		 * @param	line		Line from file with
		 * 				BTC exchange rate history.
		 * @param	first_column	What column in the DB
		 * 				is first:
		 * 				"date" or "exchange_rate".
		 */
		void append_history(const std::string & line,
				const enum e_first_db_column & first_column);

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange & src);
		BitcoinExchange & operator = (const BitcoinExchange & src);
		virtual ~BitcoinExchange();

		/**
		 * An exception that's thrown when
		 * `BitcoinExchange::read_history(std::ifstream &)`
		 * encounters some history file corruption.
		 * @class	HistoryFileIsCorrupted
		 */
		class HistoryFileIsCorrupted: public std::exception
		{
			private:
				std::string msg;

			public:
				HistoryFileIsCorrupted(const char * msg);
				HistoryFileIsCorrupted(const std::string & msg);
				HistoryFileIsCorrupted(const HistoryFileIsCorrupted & src);
				HistoryFileIsCorrupted & operator = (
						const HistoryFileIsCorrupted & src);
				virtual ~HistoryFileIsCorrupted() throw();

				virtual const char * what() const throw();
		};

		/**
		 * An exception that's thrown when
		 * `BitcoinExchange::get_date(std::string &)`
		 * encounters invalid argument (date).
		 * @class	InvalidDate
		 */
		class InvalidDate: public std::exception
		{
			private:
				std::string msg;

			public:
				InvalidDate(const char * msg);
				InvalidDate(const std::string & msg);
				InvalidDate(const InvalidDate & src);
				InvalidDate & operator = (
						const InvalidDate & src);
				virtual ~InvalidDate() throw();

				virtual const char * what() const throw();
		};

		/**
		 * An exception that's thrown when
		 * `BitcoinExchange::get_btc_exchange_rate(const std::string &)`
		 * is called, yet there's no available lower date in `history`
		 * ("Be careful to use the lower date and not the upper one").
		 * @class	NoAvailableLowerDate
		 */
		class NoAvailableLowerDate: public std::exception
		{
			private:
				std::string msg;

			public:
				NoAvailableLowerDate(const char * msg);
				NoAvailableLowerDate(const std::string & msg);
				NoAvailableLowerDate(const NoAvailableLowerDate & src);
				NoAvailableLowerDate & operator = (
						const NoAvailableLowerDate & src);
				virtual ~NoAvailableLowerDate() throw();

				virtual const char * what() const throw();
		};

		/**
		 * Get the substring with date in format "YYYY-MM-DD"
		 * stored in \p line at the beginning.
		 * @throw	InvalidDate	Date in \p line is invalid.
		 * @param	line			Line with some data,
		 * 					with date at the beginning
		 * 					in format "YYYY-MM-DD".
		 * @param	processed_bytes_in_line	Out parameter:
		 * 					amount of bytes
		 * 					processed in \p line.
		 * @return	Substring from \p line with date
		 * 		in format "YYYY-MM-DD".
		 */
		std::string get_date_substr(const std::string & line,
				size_t & processed_bytes_in_line) const;

		/**
		 * Read history of BTC exchange rate from \p file.
		 * @warning	If history was already read,
		 * 		calling this method would
		 * 		erase previous entries.
		 * @throw	HistoryFileIsCorrupted	\p file is somehow
		 * 					corrupted and couldn't
		 * 					be read and parsed.
		 * @param	file	File with BTC exchange rate
		 * 			across multiple years.
		 */
		void read_history(std::ifstream & file);

		/**
		 * Get the BTC exchange rate on \p date.
		 * If no such \p date is found,
		 * the lower closest one is returned.
		 * @throw	NoAvailableLowerDate	Only upper closest date
		 * 					to \p date was found
		 * 					or no closest date
		 * 					at all was found
		 * 					(in which case
		 * 					`history` is empty).
		 * @param	date	Date in format "YYYY-MM-DD",
		 * 			on which to find the BTC exchange rate.
		 * @return	BTC exchange rate for \p date
		 * 		or for the lower closest date,
		 * 		if \p is not found in `history`.
		 */
		double get_btc_exchange_rate(const std::string & date) const;
};

#endif	// BITCOINEXCHANGE_HPP
