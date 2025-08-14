// TODO: Rename this file to "PmergeMe.tpp".

#include <string>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <ostream>
#include <cstddef>
#include <iterator>
#include <algorithm>
#include <time.h>

namespace PmergeMe
{
	template <typename T>
	static T parse_arg(char * arg)
	{
		T ret;
		long int to_push_back;
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
				|| !(to_push_back > 0)
				|| to_push_back > std::numeric_limits<int>::max())
			{
				throw std::invalid_argument(MSG_PREFIX
						+ "Some number in an argument "
						+ "is invalid.");
			}
			ret.push_back(static_cast<int>(to_push_back));
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
	void print_container(std::ostream & os, Iterator begin, Iterator end)
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
	bool PmergeMe<Sort_Container>::CompareMainChainPairWithValue::operator () (
			const Sort_Container & pair, int value) const
	{
		return *pair.begin() < value;
	}

	template <typename Sort_Container>
	std::size_t PmergeMe<Sort_Container>::populate_a_and_b(
			const Sort_Container & SRC,
			Sort_Container * & a, Sort_Container * & b)
	{
		a = NULL;
		b = NULL;
		const std::size_t A_AND_B_SIZE = SRC.size() / 2;

		if (A_AND_B_SIZE == 0)
		{
			return 0;
		}
		a = new Sort_Container[A_AND_B_SIZE];
		try
		{
			b = new Sort_Container[A_AND_B_SIZE];
		}
		catch (const std::bad_alloc & e)
		{
			delete [] a;
			a = NULL;
			throw std::bad_alloc();
		}
		for (std::size_t i = 0; i < A_AND_B_SIZE; i++)
		{
			typename Sort_Container::const_iterator it_a;
			typename Sort_Container::const_iterator it_b;

			it_a = SRC.begin();
			std::advance(it_a, i * 2);
			it_b = it_a;
			++it_b;
			try
			{
				if (*it_a < *it_b)
				{
					a[i].push_back(*it_a);
					a[i].push_back(i * 2);
					b[i].push_back(*it_b);
					b[i].push_back(i * 2 + 1);
				}
				else
				{
					a[i].push_back(*it_b);
					a[i].push_back(i * 2 + 1);
					b[i].push_back(*it_a);
					b[i].push_back(i * 2);
				}
			}
			catch (const std::bad_alloc & e)
			{
				delete [] a;
				a = NULL;
				delete [] b;
				b = NULL;
				throw std::bad_alloc();
			}
		}
		return A_AND_B_SIZE;
	}

	template <typename Sort_Container>
	void PmergeMe<Sort_Container>::reorder_a_and_b(
			const Sort_Container NEW_ORDER,
			Sort_Container * & old_a, Sort_Container * & old_b,
			Sort_Container * & new_a, Sort_Container * & new_b)
	{
		const std::size_t NEW_A_AND_B_SIZE = NEW_ORDER.size();

		if (NEW_A_AND_B_SIZE == 0)
		{
			new_a = NULL;
			new_b = NULL;
			return;
		}
		new_a = new Sort_Container[NEW_A_AND_B_SIZE];
		try
		{
			new_b = new Sort_Container[NEW_A_AND_B_SIZE];
		}
		catch (const std::bad_alloc & e)
		{
			delete [] new_a;
			new_a = NULL;
			throw std::bad_alloc();
		}
		size_t i = 0;
		for (typename Sort_Container::const_iterator it = NEW_ORDER.begin();
				it != NEW_ORDER.end();
				++it, i++)
		{
			try
			{
				new_a[i] = old_a[*it];
				new_b[i] = old_b[*it];
			}
			catch (const std::bad_alloc & e)
			{
				delete [] new_a;
				new_a = NULL;
				delete [] new_b;
				new_b = NULL;
				throw std::bad_alloc();
			}
		}
	}

	template <typename Sort_Container>
	void PmergeMe<Sort_Container>::populate_main_chain(
			Sort_Container * & main_chain,
			const std::size_t MAIN_CHAIN_SIZE,
			const Sort_Container * a,
			const Sort_Container * b,
			const std::size_t A_AND_B_SIZE)
	{
		main_chain = NULL;

		if (A_AND_B_SIZE == 0
			|| MAIN_CHAIN_SIZE < A_AND_B_SIZE * 2)
		{
			return;
		}
		main_chain = new Sort_Container[MAIN_CHAIN_SIZE];
		try
		{
			// First element from `a`.
			main_chain[0] = a[0];
			for (size_t i = 0; i < A_AND_B_SIZE; i++)
			{
				// `+ 1`, since we've just inserted
				// a first element from `a`.
				main_chain[i + 1] = b[i];
			}
		}
		catch (const std::bad_alloc & e)
		{
			delete [] main_chain;
			main_chain = NULL;
			throw std::bad_alloc();
		}
	}

	template <typename Sort_Container>
	void PmergeMe<Sort_Container>::populate_pend(Sort_Container * & pend,
			const Sort_Container * a,
			const std::size_t A_SIZE)
	{
		pend = NULL;

		if (A_SIZE <= 1)
		{
			return;
		}
		pend = new Sort_Container[A_SIZE - 1];
		for (std::size_t i = 0; i < A_SIZE - 1; i++)
		{
			typename Sort_Container::const_iterator it;

			try
			{
				it = a[i + 1].begin();
				pend[i].push_back(*it);
				pend[i].push_back(*(++it));
				// `+ 2` to skip first element from
				// `a` and first element from `b`
				// in "main chain".
				pend[i].push_back(i + 2);
			}
			catch (const std::bad_alloc & e)
			{
				delete [] pend;
				pend = NULL;
				throw std::bad_alloc();
			}
		}
	}

	template <typename Sort_Container>
	void PmergeMe<Sort_Container>::populate_insertion_sequence(
			Sort_Container & insertion_sequence,
			const std::size_t PEND_SIZE)
	{
		insertion_sequence.clear();
		size_t jn_i;

		// Populate `this->jn` with first two Jacobsthal numbers.
		if (this->jn.empty())
		{
			this->jn.push_back(0);
			this->jn.push_back(1);
		}
		// Starting with 3rd number of Jacobsthal sequence (3).
		jn_i = 3;
		while (insertion_sequence.size() < PEND_SIZE)
		{
			// "Upper" and "lower", respectively.
			typename Sort_Container::const_iterator it_u;
			typename Sort_Container::const_iterator it_l;
			int upper, lower;

			// Apending `this->jn`.
			while (!(this->jn.size() > jn_i))
			{
				// "Last" and "penultimate", respectively.
				typename Sort_Container::const_iterator it_l;
				typename Sort_Container::const_iterator it_p;

				// By this point, we're guaranteed
				// to have at least 2 numbers in `this->jn`.
				it_l = this->jn.begin();
				std::advance(it_l, this->jn.size() - 1);
				it_p = this->jn.begin();
				std::advance(it_p, this->jn.size() - 2);
				this->jn.push_back(*it_l + 2 * *it_p);
			}
			it_u = this->jn.begin();
			std::advance(it_u, jn_i);
			upper = *it_u - 1;
			it_l = this->jn.begin();
			std::advance(it_l, jn_i - 1);
			lower = *it_l;
			jn_i++;
			if (static_cast<int>(PEND_SIZE) < upper)
			{
				upper = static_cast<int>(PEND_SIZE);
			}
			while (upper >= lower)
			{
				insertion_sequence.push_back(upper-- - 1);
			}
		}
	}

	template <typename Sort_Container>
	std::size_t PmergeMe<Sort_Container>::insert_pair_to_main_chain(
			Sort_Container * main_chain,
			const int LOWER_BOUND_HI,
			const std::size_t MAIN_CHAIN_ITEMS,
			int num, int idx)
	{
		Sort_Container * insert_pos;
		std::size_t insert_i;

		if (LOWER_BOUND_HI != -1)
		{
			insert_pos = std::lower_bound(main_chain,
					main_chain + static_cast<std::size_t>(
						LOWER_BOUND_HI),
					num,
					CompareMainChainPairWithValue());
		}
		else
		{
			insert_pos = std::lower_bound(main_chain,
					main_chain + MAIN_CHAIN_ITEMS,
					num,
					CompareMainChainPairWithValue());
		}
		insert_i = static_cast<std::size_t>(std::distance(main_chain,
					insert_pos));
		for (std::size_t i = MAIN_CHAIN_ITEMS; i > insert_i; i--)
		{
			main_chain[i] = main_chain[i - 1];
		}
		main_chain[insert_i].clear();
		main_chain[insert_i].push_back(num);
		main_chain[insert_i].push_back(idx);
		return insert_i;
	}

	template <typename Sort_Container>
	void PmergeMe<Sort_Container>::insert_pend_to_main_chain(
			Sort_Container * main_chain,
			std::size_t main_chain_items,
			Sort_Container * pend,
			const Sort_Container & insertion_sequence)
	{
		for (typename Sort_Container::const_iterator it_i = insertion_sequence.begin();
				it_i != insertion_sequence.end(); ++it_i)
		{
			// Number, index in the original sequence
			// and upper boundary
			// for `std::lower_bound()` in `main_chain`.
			typename Sort_Container::const_iterator it_p_num;
			typename Sort_Container::const_iterator it_p_idx;
			typename Sort_Container::const_iterator it_p_hi;
			size_t inserted_pos;

			it_p_num = pend[*it_i].begin();
			it_p_idx = it_p_num;
			++it_p_idx;
			it_p_hi = it_p_idx;
			++it_p_hi;
			inserted_pos = this->insert_pair_to_main_chain(
					main_chain, *it_p_hi,
					main_chain_items++,
					*it_p_num, *it_p_idx);
			// Increasing index of bigger element in `main_chain`
			// of each number in `pend`.
			for (std::size_t pend_i = 0;
					pend_i < insertion_sequence.size();
					pend_i++)
			{
				typename Sort_Container::iterator it_p;

				it_p = pend[pend_i].begin();
				std::advance(it_p, 2);
				if (*it_p >= static_cast<int>(inserted_pos))
				{
					(*it_p)++;
				}
			}
		}
	}

	template <typename Sort_Container>
	Sort_Container PmergeMe<Sort_Container>::get_sorted_sequence_indices(
			const Sort_Container & TO_SORT)
	{
		// Lesser and bigger elements of pairs, respectively.
		Sort_Container * a = NULL;
		Sort_Container * b = NULL;
		const std::size_t PAIRS_CNT = this->populate_a_and_b(TO_SORT,
				a, b);
		// The same story as in `a` and `b`:
		// the first element is the straggler itself,
		// the second is it's index in `TO_SORT`.
		// If `TO_SORT.size() % 2` is 0, `straggler` will remain NULL.
		Sort_Container * straggler = NULL;
		// Each element in `main_chain` is a container
		// containing pair of two elements:
		// first is a number from `TO_SORT`,
		// second is it's index in there.
		Sort_Container * main_chain = NULL;
		// Each element in `pend` is a container
		// containing three elements:
		// first is a number from `a` with it's "index in `a` >= 1"
		// 	to insert to `main_chain`,
		// second is it's index in `TO_SORT`,
		// third is it's bigger number from `b` with the same index
		// 	as first number has in `a`.
		Sort_Container * pend = NULL;
		// Sequence of indices of elements from `pend`
		// to insert to `main_chain`,
		// generated based on Jacobsthal numbers.
		Sort_Container insertion_sequence;
		Sort_Container sorted_sequence_indices;

		// Edge case.
		if (PAIRS_CNT == 0)
		{
			if (TO_SORT.size() == 1)
			{
				sorted_sequence_indices.push_back(0);
			}
			return sorted_sequence_indices;
		}
		// Straggler init.
		if (TO_SORT.size() % 2 == 1)
		{
			typename Sort_Container::const_iterator it;

			try
			{
				straggler = new Sort_Container;
			}
			catch (const std::bad_alloc & e)
			{
				delete [] a;
				delete [] b;
				throw std::bad_alloc();
			}
			it = TO_SORT.begin();
			std::advance(it, PAIRS_CNT * 2);
			straggler->push_back(*it);
			straggler->push_back(PAIRS_CNT * 2);
		}
		// Recursive call to sort bigger numbers
		// of pairs [ax, bx].
		if (!(PAIRS_CNT <= 1))
		{
			Sort_Container bigger_elements;
			Sort_Container sorted_pairs_indices;
			Sort_Container * new_a = NULL;
			Sort_Container * new_b = NULL;

			for (std::size_t i = 0; i < PAIRS_CNT; i++)
			{
				bigger_elements.push_back(*b[i].begin());
			}
			try
			{
				sorted_pairs_indices = this->get_sorted_sequence_indices(
						bigger_elements);
				this->reorder_a_and_b(sorted_pairs_indices,
						a, b, new_a, new_b);
			}
			catch (const std::bad_alloc & e)
			{
				delete [] a;
				delete [] b;
				delete straggler;
				throw std::bad_alloc();
			}
			delete [] a;
			delete [] b;
			a = new_a;
			b = new_b;
		}
		try
		{
			this->populate_main_chain(main_chain, TO_SORT.size(),
					a, b, PAIRS_CNT);
		}
		catch (const std::bad_alloc & e)
		{
			delete [] a;
			delete [] b;
			delete straggler;
			throw std::bad_alloc();
		}
		try
		{
			this->populate_pend(pend, a, PAIRS_CNT);
		}
		catch (const std::bad_alloc & e)
		{
			delete [] a;
			delete [] b;
			delete straggler;
			delete [] main_chain;
			throw std::bad_alloc();
		}
		delete [] a;
		delete [] b;
		try
		{
			if (PAIRS_CNT != 0)
			{
				this->populate_insertion_sequence(
						insertion_sequence,
						PAIRS_CNT - 1);
			}
			else
			{
				this->populate_insertion_sequence(
						insertion_sequence,
						PAIRS_CNT);
			}
		}
		catch (const std::bad_alloc & e)
		{
			delete straggler;
			delete [] main_chain;
			delete [] pend;
			throw std::bad_alloc();
		}
		try
		{
			// `PAIRS_CNT + 1`, because by default
			// first element from `a`
			// and all elements from `b` get inserted
			// into `main_chain`.
			this->insert_pend_to_main_chain(main_chain,
					PAIRS_CNT + 1,
					pend, insertion_sequence);
		}
		catch (const std::bad_alloc & e)
		{
			delete straggler;
			delete [] main_chain;
			delete [] pend;
			throw std::bad_alloc();
		}
		delete [] pend;
		if (straggler != NULL)
		{
			typename Sort_Container::const_iterator it_num;
			typename Sort_Container::const_iterator it_idx;

			it_num = straggler->begin();
			it_idx = it_num;
			++it_idx;
			try
			{
				(void) this->insert_pair_to_main_chain(
						main_chain, -1,
						PAIRS_CNT * 2,
						*it_num, *it_idx);
			}
			catch (const std::bad_alloc & e)
			{
				delete straggler;
				delete [] main_chain;
				throw std::bad_alloc();
			}
			delete straggler;
		}
		for (size_t i = 0; i < TO_SORT.size(); i++)
		{
			typename Sort_Container::const_iterator it;

			it = main_chain[i].begin();
			try
			{
				sorted_sequence_indices.push_back(*(++it));
			}
			catch (const std::bad_alloc & e)
			{
				delete [] main_chain;
				throw std::bad_alloc();
			}
		}
		delete [] main_chain;
		return sorted_sequence_indices;
	}

	template <typename Sort_Container>
	struct timediff PmergeMe<Sort_Container>::sort(
			const Sort_Container & IN)
	{
		struct timespec begin_tm;
		Sort_Container sorted_indices_sequence;
		struct timespec end_tm;
		struct timediff tm_diff;
		const std::string MSG_PREFIX = "PmergeMe<Sort_Container>::sort(): ";
		const std::string GET_TIME_FAIL_MSG = "clock_gettime() failed.";

		// "Forbidden functions: None",
		// also there's no C++98 function to accurately
		// measure time up down to microseconds.
		//
		// `clock_gettime()` provides identical functionality
		// as `gettimeofday()` does in our case,
		// it's just that `gettimeofday()` is deprecated
		// as of POSIX.1-2008.
		if (clock_gettime(CLOCK_MONOTONIC, &begin_tm) == -1)
		{
			throw GetTimeFail(MSG_PREFIX + GET_TIME_FAIL_MSG);
		}
		this->data.clear();
		sorted_indices_sequence = this->get_sorted_sequence_indices(IN);
		for (typename Sort_Container::const_iterator it_i = sorted_indices_sequence.begin();
				it_i != sorted_indices_sequence.end();
				++it_i)
		{
			typename Sort_Container::const_iterator it_val;

			it_val = IN.begin();
			std::advance(it_val, *it_i);
			this->data.push_back(*it_val);
		}
		if (clock_gettime(CLOCK_MONOTONIC, &end_tm) == -1)
		{
			throw GetTimeFail(MSG_PREFIX + GET_TIME_FAIL_MSG);
		}
		tm_diff.tv_sec = end_tm.tv_sec - begin_tm.tv_sec;
		tm_diff.tv_nsec = end_tm.tv_nsec - begin_tm.tv_nsec;
		return tm_diff;
	}

	template <typename Sort_Container>
	const Sort_Container & PmergeMe<Sort_Container>::get_result() const
	{
		return this->data;
	}
}
