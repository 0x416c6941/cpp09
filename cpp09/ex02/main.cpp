#include <vector>
#include <cstddef>
#include <string>
#include "PmergeMe.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib>

// XXX: There's no limitations or instructions
// regarding namespaces in subject.
// Therefore, I've done everything according to my own discretion.
int main(int argc, char * argv[])
{
	std::vector<unsigned int> input;
	const size_t MAX_ELEMENTS = 9999;
	const std::string	ERROR_MSG = "Error",
				OOM_MSG = "OOM";

	if (argc <= 1)
	{
		// Subject doesn't specify
		// if that's supposed to be an error or not.
		return 0;
	}
	try
	{
		input = PmergeMe::parse_argv<std::vector<unsigned int> >(
				--argc, ++argv);
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << OOM_MSG << std::endl;
		return EXIT_FAILURE;
	}
	// Stack smashing.
	if (input.size() > MAX_ELEMENTS)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
