#include <vector>
#include <cstddef>
#include <string>
#include "PmergeMe.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <deque>
#include <list>

// XXX: There's no limitations or instructions
// regarding namespaces in subject.
// Therefore, I've done everything according to my own discretion.
int main(int argc, char * argv[])
{
	std::vector<int> input;
	const std::size_t MAX_ELEMENTS = 9999;
	const std::string	ERROR_MSG = "Error",
				OOM_MSG = "OOM";

	// Subject doesn't specify
	// if that's supposed to be an error or not.
	if (argc <= 1)
	{
		return 0;
	}
	try
	{
		input = PmergeMe::parse_argv<std::vector<int> >(
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
	// Here comes a lot of duplicated code.
	// But still, by this point I believe it's not an issue.
	try
	{
		std::vector<int> to_sort;
		PmergeMe::PmergeMe<std::vector<int> > solver;
		struct PmergeMe::timediff time_taken;

		// It doesn't really matter if call
		// `end()` or `begin()` method here.
		// However, for code readability, let's just use `end()`.
		to_sort.insert(to_sort.end(), input.begin(), input.end());
		time_taken = solver.sort(to_sort);
		std::cout << "Before:\t";
		PmergeMe::print_container(
				std::cout, to_sort.begin(), to_sort.end());
		std::cout << std::endl
				<< "After:\t";
		PmergeMe::print_container(
				std::cout,
				solver.get_result().begin(),
				solver.get_result().end());
		std::cout << std::endl
				<< "Time taken to sort "
				<< to_sort.size();
		if (to_sort.size() == 1)
		{
			std::cout << " element";
		}
		else
		{
			std::cout << " elements";
		}
		std::cout << " with std::vector<int>: ";
		PmergeMe::print_timediff(std::cout, time_taken);
		std::cout << " microseconds" << std::endl;
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << OOM_MSG << std::endl;
		return EXIT_FAILURE;
	}
	catch (const PmergeMe::PmergeMe<std::vector<int> >::GetTimeFail & e)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << std::endl;
	try
	{
		std::deque<int> to_sort;
		PmergeMe::PmergeMe<std::deque<int> > solver;
		struct PmergeMe::timediff time_taken;

		// It doesn't really matter if call
		// `end()` or `begin()` method here.
		// However, for code readability, let's just use `end()`.
		to_sort.insert(to_sort.end(), input.begin(), input.end());
		time_taken = solver.sort(to_sort);
		std::cout << "Before:\t";
		PmergeMe::print_container(
				std::cout, to_sort.begin(), to_sort.end());
		std::cout << std::endl
				<< "After:\t";
		PmergeMe::print_container(
				std::cout,
				solver.get_result().begin(),
				solver.get_result().end());
		std::cout << std::endl
				<< "Time taken to sort "
				<< to_sort.size();
		if (to_sort.size() == 1)
		{
			std::cout << " element";
		}
		else
		{
			std::cout << " elements";
		}
		std::cout << " with std::deque<int>: ";
		PmergeMe::print_timediff(std::cout, time_taken);
		std::cout << " microseconds" << std::endl;
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << OOM_MSG << std::endl;
		return EXIT_FAILURE;
	}
	catch (const PmergeMe::PmergeMe<std::deque<int> >::GetTimeFail & e)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << std::endl;
	try
	{
		std::list<int> to_sort;
		PmergeMe::PmergeMe<std::list<int> > solver;
		struct PmergeMe::timediff time_taken;

		// It doesn't really matter if call
		// `end()` or `begin()` method here.
		// However, for code readability, let's just use `end()`.
		to_sort.insert(to_sort.end(), input.begin(), input.end());
		time_taken = solver.sort(to_sort);
		std::cout << "Before:\t";
		PmergeMe::print_container(
				std::cout, to_sort.begin(), to_sort.end());
		std::cout << std::endl
				<< "After:\t";
		PmergeMe::print_container(
				std::cout,
				solver.get_result().begin(),
				solver.get_result().end());
		std::cout << std::endl
				<< "Time taken to sort "
				<< to_sort.size();
		if (to_sort.size() == 1)
		{
			std::cout << " element";
		}
		else
		{
			std::cout << " elements";
		}
		std::cout << " with std::list<int>: ";
		PmergeMe::print_timediff(std::cout, time_taken);
		std::cout << " microseconds" << std::endl;
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << OOM_MSG << std::endl;
		return EXIT_FAILURE;
	}
	catch (const PmergeMe::PmergeMe<std::list<int> >::GetTimeFail & e)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
