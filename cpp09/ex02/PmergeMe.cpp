#include "PmergeMe.hpp"
#include <fstream>

namespace PmergeMe
{
	void print_timediff(
			std::ofstream & os, const struct timediff & TMDIFF)
	{
		// Microseconds.
		const double US = TMDIFF.tv_sec * 1e6 + TMDIFF.tv_nsec / 1e3;

		os << US;
	}
}
