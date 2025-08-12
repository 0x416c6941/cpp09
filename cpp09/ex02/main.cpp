#include <list>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include "PmergeMe.hpp"
#include <iostream>

/**
 * Namespace dedicated specifically to PmergeMe
 * to potentially avoid collision w/ names of other functions and classes.
 * @namespace	PmergeMe
 */
namespace PmergeMe
{
	/**
	 * Parse a single \p arg to a list of positive integers.
	 * @throw	std::invalid_argument	Received an invalid input.
	 * @param	arg	One of arguments from @c argv.
	 * @return	List of parsed integers from \p arg with
	 * 		the order of elements preserved.
	 */
	static std::list<int> parse_arg(char * arg);

	/**
	 * Parse \p argv of \p argc elements to a list of positive integers.
	 * @throw	std::invalid_argument	Received an invalid input.
	 * @param	argc	Argument count.
	 * @param	argv	Argument vector.
	 * @return	List of parsed integers from \p argv with
	 * 		the order of elements preserved.
	 */
	std::list<int> parse_argv(int argc, char * argv[]);

	/**
	 * Print container with beginning iterator \p begin
	 * and ending iterator \p end to `stdout`.
	 * @tparam	Iterator	Iterator of a container
	 * 				to print content of on `stdout`.
	 * @param	begin		Iterator of type \t T,
	 * 				which is a beginning of that container.
	 * @param	begin		Iterator of type \t T,
	 * 				which is an ending of that container.
	 */
	template <typename Iterator>
	void print_container(Iterator begin, Iterator end);

	/**
	 * Print \p TMDIFF on `stdout`.
	 * @param	TMDIFF	Time diff to print on `stdout`.
	 */
	void print_timediff(const struct PmergeMe::PmergeMe::timediff & TMDIFF);

	static std::list<int> parse_arg(char * arg)
	{
		std::list<int> ret;
		long to_push_back;
		const int STRTOL_BASE = 10;
		const std::string MSG_PREFIX = "parse_arg():: ";

		while (*arg != '\0')
		{
			// Only space was shown in subject.
			// I therefore don't handle horizontal tabs
			// and other whitespace characters other than space.
			while (*arg == ' ')
			{
				arg++;
			}
			if (*arg == '\0')
			{
				break;
			}
			errno = 0;
			to_push_back = strtol(arg, &arg, STRTOL_BASE);
			if (!(*arg == '\0' || *arg == ' ') || errno == ERANGE
				|| to_push_back <= 0
				|| to_push_back > std::numeric_limits<int>::max())
			{
				throw std::invalid_argument(MSG_PREFIX
						+ "Some number in an argument "
						+ "is invalid.");
			}
			ret.push_back(static_cast<int> (to_push_back));
		}
		if (ret.empty())
		{
			throw std::invalid_argument(MSG_PREFIX
					+ "Some argument doesn't contain "
					+ "any numbers at all.");
		}
		return ret;
	}

	std::list<int> parse_argv(int argc, char * argv[])
	{
		std::list<int> ret, to_splice;
		const std::string MSG_PREFIX = "parse_argv():: ";

		// Single arguments in `argv`.
		for (int i = 0; i < argc; i++)
		{
			to_splice = parse_arg(argv[i]);
			ret.splice(ret.end(), to_splice);
		}
		// Checking for duplicates.
		for (std::list<int>::const_iterator it1 = ret.begin();
				it1 != ret.end(); ++it1)
		{
			std::list<int>::const_iterator it2 = it1;

			std::advance(it2, 1);
			for (; it2 != ret.end(); ++it2)
			{
				if (*it1 == *it2)
				{
					throw std::invalid_argument(MSG_PREFIX
							+ "Some number "
							+ "is a duplicate.");
				}
			}
		}
		return ret;
	}

	template <typename Iterator>
	void print_container(Iterator begin, Iterator end)
	{
		bool first_element = true;

		while (begin != end)
		{
			if (!first_element)
			{
				std::cout << ' ';
			}
			first_element = false;
			std::cout << *begin;
			std::advance(begin, 1);
		}
	}

	void print_timediff(const struct PmergeMe::PmergeMe::timediff & TMDIFF)
	{
		// Microseconds.
		double us = TMDIFF.tv_sec * 1e6 + TMDIFF.tv_nsec / 1e3;

		std::cout << us;
	}
}

// XXX: There's no limitations or instructions
// regarding namespaces in subject.
// Therefore, I've done everything according to my own discretion.
int main(int argc, char * argv[])
{
	std::list<int> input, test;
	const size_t MAX_ELEMENTS = 9999;
	PmergeMe::PmergeMe solver;
	struct PmergeMe::PmergeMe::timediff time_taken;
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
		input = PmergeMe::parse_argv(--argc, ++argv);
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
	// Prevent possible stack smashing.
	if (input.size() > MAX_ELEMENTS)
	{
		std::cerr << ERROR_MSG << std::endl;
		return EXIT_FAILURE;
	}
	time_taken = solver.sort(input, test);
	// "Before:\t", "After:\t".
	std::cout << "Before:\t";
	PmergeMe::print_container(input.begin(), input.end());
	std::cout << std::endl
			<< "After:\t";
	PmergeMe::print_container(test.begin(), test.end());
	// Time taken to sort with first container.
	std::cout << std::endl
			<< "Time taken to sort with std::list<int>:\t";
	PmergeMe::print_timediff(time_taken);
	std::cout << " us" << std::endl;
	// Comparison operations taken to sort the data
	// with first container.
	std::cout << "Comparison OPs taken to sort with std::list<int>:\t"
			<< solver.get_comparison_count() << std::endl;
	return 0;
}
