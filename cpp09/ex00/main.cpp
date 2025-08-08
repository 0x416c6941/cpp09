#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>

int main(int argc, char * argv[])
{
	const int EXPECTED_ARGS = 2, FILE_PATH = 1;
	const std::string OPEN_FAIL = "Error: could not open file.";
	std::ifstream file;

	if (argc != EXPECTED_ARGS)
	{
		std::cerr << OPEN_FAIL << std::endl;
		return EXIT_FAILURE;
	}
	file.open(argv[FILE_PATH]);
	if (!file.is_open())
	{
		std::cerr << OPEN_FAIL << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
