#include <string>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <vector>
#include <cstddef>
#include <iostream>
//#include "PmergeMe.hpp"

/**
 * Namespace dedicated specifically to PmergeMe
 * to potentially avoid collision w/ names of other functions and classes.
 * @namespace	PmergeMe
 */
namespace PmergeMe
{
	/**
	 * The time difference in seconds and nanoseconds
	 * between end time and begin time of sorting process,
	 * since it's not guaranteed by standard that
	 * `tv_sec` and `tv_nsec` of `struct timespec`
	 * can hold negative values.
	 * @struct	timediff
	 */
	struct timediff
	{
		long tv_sec;
		long tv_nsec;
	};

	/**
	 * Parse a single \p arg to a \t T of unsigned integers.
	 * @warning	If type stored in \t T isn't `unsigned int`,
	 * 		the correct behavior of this function
	 * 		isn't guaranteed.
	 * @tparam	T	Container, that saves the order
	 * 			of inserted elements
	 * 			and provides `push_back()`
	 * 			and `empty()` methods.
	 * @throw	std::bad_alloc		Some allocation failed.
	 * @throw	std::invalid_argument	Received an invalid input.
	 * @param	arg	One of arguments from @c argv.
	 * @return	\t T of parsed unsigned integers from \p arg
	 * 		with the order of elements preserved.
	 */
	template <typename T>
	static T parse_arg(char * arg);

	/**
	 * Parse \p argv of \p argc elements to a \t T of unsigned integers.
	 * @warning	If type stored in \t T isn't `unsigned int`,
	 * 		the correct behavior of this function
	 * 		isn't guaranteed.
	 * @tparam	T	Container, that saves the order
	 * 			of inserted elements
	 * 			and provides `insert()`,
	 * 			`push_back()` and `empty()` methods,
	 * 			as well as `const_iterator`
	 * 			and `begin()` and `end()` methods
	 * 			to obtain it.
	 * @throw	std::bad_alloc		Some allocation failed.
	 * @throw	std::invalid_argument	Received an invalid input.
	 * @param	argc	Argument count.
	 * @param	argv	Argument vector.
	 * @return	\t T of parsed unsigned integers from \p argv
	 * 		with the order of elements preserved.
	 */
	template <typename T>
	T parse_argv(int argc, char * argv[]);

	/**
	 * Print container with beginning iterator \p begin
	 * and ending iterator \p end to `stdout`.
	 * @tparam	Iterator	Iterator of a container
	 * 				to print content of on `stdout`.
	 * @param	begin		Iterator of type \t T,
	 * 				which is a beginning of that container.
	 * @param	end		Iterator of type \t T,
	 * 				which is an ending of that container.
	 */
	template <class Iterator>
	void print_container(Iterator begin, Iterator end);

	/**
	 * Print \p TMDIFF on `stdout`.
	 * @param	TMDIFF	Time diff to print on `stdout`.
	 */
	void print_timediff(const struct timediff & TMDIFF);

	template <typename T>
	static T parse_arg(char * arg)
	{
		T ret;
		unsigned long int to_push_back;
		const int STRTOUL_BASE = 10;
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
			to_push_back = strtoul(arg, &arg, STRTOUL_BASE);
			if (!(*arg == '\0' || *arg == ' ') || errno == ERANGE
				|| to_push_back > std::numeric_limits<unsigned int>::max())
			{
				throw std::invalid_argument(MSG_PREFIX
						+ "Some number in an argument "
						+ "is invalid.");
			}
			ret.push_back(static_cast<unsigned int>(to_push_back));
		}
		if (ret.empty())
		{
			throw std::invalid_argument(MSG_PREFIX
					+ "Some argument doesn't contain "
					+ "any numbers at all.");
		}
		return ret;
	}

	template <typename T>
	T parse_argv(int argc, char * argv[])
	{
		T ret, to_insert_at_the_end;
		const std::string MSG_PREFIX = "parse_argv():: ";

		// Single arguments in `argv`.
		for (int i = 0; i < argc; i++)
		{
			to_insert_at_the_end = parse_arg<T>(argv[i]);
			ret.insert(ret.end(),
					to_insert_at_the_end.begin(),
					to_insert_at_the_end.end());
		}
		// Checking for duplicates.
		for (typename T::const_iterator it1 = ret.begin();
				it1 != ret.end();
				++it1)
		{
			typename T::const_iterator it2;

			it2 = it1;
			++it2;
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

	template <class Iterator>
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

	void print_timediff(const struct timediff & TMDIFF)
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
