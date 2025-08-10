#include <string>
#include <iostream>
#include <cstdlib>

int main(int argc, char * argv[])
{
	const int EXPECTED_ARGS = 2, EXPRESSION = 1;
	const std::string ERROR_MSG = "Error";

	if (argc != EXPECTED_ARGS)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	(void) EXPRESSION;
	(void) argv;
	return 0;
}
