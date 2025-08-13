// TODO: Rename this file to "PmergeMe.tpp".

#include "PmergeMe.hpp"
#include <string>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <fstream>

namespace PmergeMe
{
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
				|| !(to_push_back > 0)
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
	void print_container(std::ofstream & os, Iterator begin, Iterator end)
	{
		bool first_element = true;

		while (begin != end)
		{
			if (!first_element)
			{
				os << ' ';
			}
			first_element = false;
			os << *begin;
			std::advance(begin, 1);
		}
	}

	template <typename Sort_Container>
	PmergeMe<Sort_Container>::PmergeMe()
	{
	}

	/*
	template <typename Sort_Container>
	PmergeMe<Sort_Container>::PmergeMe(
			const PmergeMe<Sort_Container> & src)
		: data(src.data)
	{
	}

	template <typename Sort_Container>
	PmergeMe<Sort_Container> & PmergeMe<Sort_Container>::operator = (
			const PmergeMe<Sort_Container> & src)
	{
		if (this == &src)
		{
			return *this;
		}
		this->data = src.data;
		return *this;
	}
	 */

	template <typename Sort_Container>
	PmergeMe<Sort_Container>::~PmergeMe()
	{
	}

	template <typename Sort_Container>
	PmergeMe<Sort_Container>::GetTimeFail::GetTimeFail(const char * msg)
		: msg(msg)
	{
	}

	template <typename Sort_Container>
	PmergeMe<Sort_Container>::GetTimeFail::GetTimeFail(
			const std::string & msg)
		: msg(msg)
	{
	}

	template <typename Sort_Container>
	PmergeMe<Sort_Container>::GetTimeFail::GetTimeFail(
			const PmergeMe<Sort_Container>::GetTimeFail & src)
		: msg(src.msg)
	{
	}

	template <typename Sort_Container>
	typename PmergeMe<Sort_Container>::GetTimeFail & PmergeMe<Sort_Container>::GetTimeFail::operator = (
			const PmergeMe<Sort_Container>::GetTimeFail & src)
	{
		if (this == &src)
		{
			return *this;
		}
		this->msg = src.msg;
		return *this;
	}

	template <typename Sort_Container>
	PmergeMe<Sort_Container>::GetTimeFail::~GetTimeFail() throw()
	{
	}

	template <typename Sort_Container>
	const char * PmergeMe<Sort_Container>::GetTimeFail::what() const throw()
	{
		return this->msg.c_str();
	}

	template <typename Sort_Container>
	template <typename T>
	struct timediff PmergeMe<Sort_Container>::sort(const T & what)
	{
		(void) what;
	}

	template <typename Sort_Container>
	const Sort_Container & PmergeMe<Sort_Container>::get_result() const
	{
		return this->data;
	}
}
