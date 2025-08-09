#ifndef BTC_HPP
#define BTC_HPP

#include "BTCExchangeHistory.hpp"
#include <fstream>

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
int convert(const BTCExchangeHistory & exchange_history, std::ifstream & file);

#endif	// BTC_HPP
