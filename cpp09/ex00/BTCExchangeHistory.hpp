#ifndef BTCEXCHANGEHISTORY_HPP
#define BTCEXCHANGEHISTORY_HPP

#include <map>
#include "Date.hpp"
#include <stdexcept>
#include <string>
#include <fstream>

/**
 * A class to parse and store BTC exchange rate across multiple years.
 * @class	BTCExchangeHistory
 */
class BTCExchangeHistory
{
	private:
		std::map<Date, double> history;

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
		 * 				which column in the DB is first.
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
		BTCExchangeHistory();
		BTCExchangeHistory(const BTCExchangeHistory & src);
		BTCExchangeHistory & operator = (const BTCExchangeHistory & src);
		virtual ~BTCExchangeHistory();

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

		class HistoryIsEmpty: public std::exception
		{
			private:
				std::string msg;

			public:
				HistoryIsEmpty(const char * msg);
				HistoryIsEmpty(const std::string & msg);
				HistoryIsEmpty(const HistoryIsEmpty & src);
				HistoryIsEmpty & operator = (
						const HistoryIsEmpty & src);
				virtual ~HistoryIsEmpty() throw();

				virtual const char * what() const throw();
		};

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
		 * If no such \p date is found, the closest one is found.
		 * If there are two closest (lower and upper) dates,
		 * the lower one will be chosen.
		 * @throw	HistoryIsEmpty	`history` is empty.
		 * @param	date	Date, on which to find
		 * 			the BTC exchange rate.
		 * @return	BTC exchange rate for \p date
		 * 		or for the closest date, if \p is not found.
		 */
		double get_btc_exchange_rate(const Date & date);
};

#endif	// BTCEXCHANGEHISTORY_HPP
