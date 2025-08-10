#include "RPN.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main(int argc, char * argv[])
{
	const int EXPECTED_ARGS = 2, EXPRESSION = 1;
	const std::string ERROR_MSG = "Error";

	if (argc != EXPECTED_ARGS)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		std::cout << RPN::evaluate(argv[EXPRESSION]) << std::endl;
	}
	catch (const RPN::InvalidExpression & e)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
