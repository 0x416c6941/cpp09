#include <list>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <iterator>
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
}

// XXX: There's no limitations or instructions
// regarding namespaces in subject.
// Therefore, I've done everything according to my own discretion.
int main(int argc, char * argv[])
{
	std::list<int> input;
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
	return 0;
}
