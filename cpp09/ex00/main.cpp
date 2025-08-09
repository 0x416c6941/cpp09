#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "BTCExchangeHistory.hpp"
#include <stdexcept>
#include "btc.hpp"

// Ideally, to prevent collision with already existing projects,
// we should've been defined a namespace for our functions and classes.
// Still, at this point everything works and I don't really care so much.
int main(int argc, char * argv[])
{
	const int EXPECTED_ARGS = 2;
	const std::string INPUT_FILE_OPEN_FAIL = "Error: could not open file.";
	const std::string DB_FILE = "data.csv";
	const int FILE_PATH = 1;
	std::ifstream file;
	BTCExchangeHistory btc_exchange_history;

	if (argc != EXPECTED_ARGS)
	{
		std::cerr << INPUT_FILE_OPEN_FAIL << std::endl;
		return EXIT_FAILURE;
	}
	file.open(DB_FILE.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: Couldn't open the DB file "
				<< DB_FILE << '.' << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		btc_exchange_history.read_history(file);
	}
	catch (const BTCExchangeHistory::HistoryFileIsCorrupted & e)
	{
		std::cerr << "Error: DB file "
				<< DB_FILE << " is corrupted." << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << "Error: OOM." << std::endl;
		return EXIT_FAILURE;
	}
	file.close();
	file.open(argv[FILE_PATH]);
	if (!file.is_open())
	{
		std::cerr << INPUT_FILE_OPEN_FAIL << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		return convert(btc_exchange_history, file);
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << "Error: OOM." << std::endl;
		return EXIT_FAILURE;
	}
}
