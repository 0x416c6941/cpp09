#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <ctime>

// TODO: Remove.
#include <iostream>

namespace PmergeMe
{
	/**
	 * An implementation of merge-insertion sort.
	 * @class	PmergeMe
	 */
	class PmergeMe
	{
		private:
			// It doesn't make sense for `PmergeMe`
			// neither to be CopyConstructible,
			// nor to have `operator = ()` overloaded.
			PmergeMe(const PmergeMe & src);
			PmergeMe & operator = (const PmergeMe & src);

			/**
			 * An exception that's thrown when
			 * `PmergeMe::are_pairs_sorted_by_second_element(
			 * 	Iterator, Iterator)`
			 * finds container that doesn't contain a pair
			 * (isn't of size 2).
			 * @class	InvalidPairSize
			 */
			class InvalidPairSize: public std::exception
			{
				private:
					std::string msg;

				public:
					InvalidPairSize(const char * msg);
					InvalidPairSize(const std::string & msg);
					InvalidPairSize(const InvalidPairSize & src);
					InvalidPairSize & operator = (
							const InvalidPairSize & src);
					virtual ~InvalidPairSize() throw();

					virtual const char * what() const throw();
			};

			// Count of element's comparisons.
			size_t comp_cnt;

			/**
			 * Split \p IN to pairs.
			 * If element count in \p IN is odd,
			 * last element will remain untouched.
			 * @warning	Type stored in \t T
			 * 		must be @c CopyConstructible.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `push_back()`,
			 * 			and `size()` methods,
			 * 			`const_iterator` to
			 * 			iterate through it,
			 * 			and `begin()` method
			 * 			to obtain such iterator.
			 * @param	IN	Container to split
			 * 			into pairs with sorted content.
			 * @return	Dynamic array of containers
			 * 		of type \t T, each containing
			 * 		a pair of unsorted content from \p IN.
			 * 		Size of this array is `IN.size() / 2`.
			 */
			template <typename T>
			T * get_pairs_with_unsorted_content(const T & IN)
			{
				// `IN.size() / 2` because
				// there will be `IN.size() / 2` pairs.
				const size_t RET_SIZE = IN.size() / 2;
				T * ret = new T[RET_SIZE];

				for (size_t i = 0; i < RET_SIZE; i++)
				{
					typename T::const_iterator it = IN.begin();
					typename T::const_iterator next_it;

					// `i * 2` because
					// pair consists of two elements.
					std::advance(it, i * 2);
					next_it = it;
					std::advance(next_it, 1);
					ret[i].push_back(*it);
					ret[i].push_back(*next_it);
				}
				return ret;
			}

			/**
			 * Split \p IN to pairs and sort content of each pair.
			 * If element count in \p IN is odd,
			 * last element will remain untouched.
			 * @warning	Type stored in \t T
			 * 		must have `operator < ()`
			 * 		overloaded for it
			 * 		and be @c CopyConstructible.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `push_back()`,
			 * 			and `size()` methods,
			 * 			`const_iterator`,
			 * 			and `iterator` to
			 * 			iterate through it,
			 * 			and `begin()` method
			 * 			to obtain such iterators.
			 * @param	IN	Container to split
			 * 			into pairs with sorted content.
			 * @return	Dynamic array of containers
			 * 		of type \t T, each containing
			 * 		a pair of sorted content from \p IN.
			 * 		Size of this array is `IN.size() / 2`.
			 */
			template <typename T>
			T * get_pairs_with_sorted_content(const T & IN)
			{
				// there will be `IN.size() / 2` pairs.
				const size_t RET_SIZE = IN.size() / 2;
				T * ret = this->get_pairs_with_unsorted_content(IN);

				for (size_t i = 0; i < RET_SIZE; i++)
				{
					typename T::iterator it;
					typename T::iterator next_it;

					it = ret[i].begin();
					next_it = it;
					// `PmergeMe::get_pairs_with_unsorted_content()`
					// will for sure return us a dynamic array
					// of containers, each of size 2.
					// Hence not checking if `next_it`
					// is within the bounds of `ret[i]`.
					std::advance(next_it, 1);
					this->comp_cnt++;
					if (!(*it < *next_it))
					{
						std::iter_swap(it, next_it);
					}
				}
				return ret;
			}

			/**
			 * Check if pairs
			 * returned by `get_pairs_with_sorted_content()`
			 * are sorted by second element in each pair.
			 * @warning	Type stored in \t T
			 * 		must have `operator < ()`
			 * 		overloaded for it.
			 * 		If type stored in \t T is one with
			 * 		floating-point, it's up to you
			 * 		to ensure `operator == ()`
			 * 		is precise enough.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `size()` method,
			 * 			`const_iterator` to
			 * 			iterate through it,
			 * 			and `begin()` method
			 * 			to obtain such iterator.
			 * @param	pairs		Pointer to containers
			 * 				of type \t T,
			 * 				each containing
			 * 				a pair of elements.
			 * @param	PAIRS_CNT	Size of \p pairs.
			 * @return	true, if yes.
			 * @return	false, if no.
			 */
			template <typename T>
			bool are_pairs_sorted_by_second_element(const T * pairs,
					const size_t PAIRS_CNT)
			{
				const std::string MSG_PREFIX = "PmergeMe::are_pairs_sorted_by_second_element(): ";

				if (PAIRS_CNT <= 1)
				{
					return true;
				}
				// We've checked if `PAIRS_CNT` is at least 2,
				// `PAIRS_CNT - 1` won't result in underflow.
				for (size_t i = 0; i < PAIRS_CNT - 1; i++)
				{
					typename T::const_iterator it_second_elem;
					typename T::const_iterator next_it_second_elem;

					if (pairs[i].size() != 2
						|| pairs[i + 1].size() != 2)
					{
						throw InvalidPairSize(MSG_PREFIX
								+ "Some container doesn't "
								+ "contain a pair.");
					}
					it_second_elem = pairs[i].begin();
					std::advance(it_second_elem, 1);
					next_it_second_elem = pairs[i + 1].begin();
					std::advance(next_it_second_elem, 1);
					this->comp_cnt++;
					if (!(*it_second_elem < *next_it_second_elem))
					{
						return false;
					}
				}
				return true;
			}

			/**
			 * Populate main chain with:
			 * lesser element from first pair,
			 * larger elements from all pairs.
			 * @warning	It's up to you to ensure
			 * 		each container
			 * 		in \p pairs_with_sorted_content
			 * 		is actually a pair.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `push_back()`,
			 * 			`const_iterator` to
			 * 			iterate through it,
			 * 			and `begin()`, and `end()`
			 * 			methods to obtain
			 * 			such iterators.
			 * @param	pairs		Pointer to containers
			 * 				of type \t T,
			 * 				each containing
			 * 				a sorted pair of elements.
			 * @param	PAIRS_CNT	Size of \p pairs.
			 * @return	Main chain suitable
			 * 		for "insertion" phase.
			 */
			template <typename T>
			T populate_main_chain(const T * pairs_with_sorted_content,
					const size_t PAIRS_CNT)
			{
				T ret;

				if (PAIRS_CNT == 0)
				{
					return ret;
				}
				// Lesser element from first pair.
				ret.push_back(*pairs_with_sorted_content[0].begin());
				// Larger elements from all pairs.
				for (size_t i = 0; i < PAIRS_CNT; i++)
				{
					typename T::const_iterator it_second_elem;

					it_second_elem = pairs_with_sorted_content[i].begin();
					std::advance(it_second_elem, 1);
					ret.push_back(*it_second_elem);
				}
				return ret;
			}

			/**
			 * Basically the core of the class:
			 * a method to sort \p IN with Merge-insertion.
			 * @warning	Type stored in \t T
			 * 		must have `operator < ()`
			 * 		overloaded for it
			 * 		and be @c CopyConstructible.
			 * 		If type stored in \t T is one with
			 * 		floating-point, it's up to you
			 * 		to ensure `operator == ()`
			 * 		is precise enough.
			 * @warning	\t T must have `operator == ()`
			 * 		overloaded for it.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `push_back()`,
			 * 			`erase()`, and `size()` methods,
			 * 			`const_iterator`,
			 * 			and `iterator` to
			 * 			iterate through it,
			 * 			and `begin()` method
			 * 			to obtain
			 * 			such iterators.
			 * @param	IN	Container to sort.
			 * @return	\p IN sorted.
			 */
			template <typename T>
			T merge_insert(const T & IN)
			{
				const std::string MSG_PREFIX = "PmergeMe::merge_insert(): ";
				T * pairs_with_sorted_content;
				size_t pairs_cnt;
				T main_chain, pend;

				pairs_with_sorted_content = this->get_pairs_with_sorted_content(
						IN);
				pairs_cnt = IN.size() / 2;
				// Edge case, although should never happen.
				if (pairs_cnt == 0)
				{
					return IN;
				}
				else if (!are_pairs_sorted_by_second_element(
						pairs_with_sorted_content,
						pairs_cnt))
				{
					std::cout << "LOG: Pairs aren't sorted." << std::endl;
					delete [] pairs_with_sorted_content;
					return T();
				}
				try
				{
					main_chain = this->populate_main_chain(
							pairs_with_sorted_content,
							pairs_cnt);
				}
				catch (const std::bad_alloc & e)
				{
					delete [] pairs_with_sorted_content;
					throw std::bad_alloc();
				}
				delete [] pairs_with_sorted_content;
				return main_chain;
			}

		public:
			PmergeMe();
			virtual ~PmergeMe();

			/**
			 * The time difference in seconds and nanoseconds
			 * between end time and begin time of sorting process,
			 * since it's not guaranteed by standard that
			 * `tv_sec` and `tv_nsec` of `struct timespec`
			 * can hold negative values.
			 */
			struct timediff
			{
				long tv_sec;
				long tv_nsec;
			};

			/**
			 * An exception that's thrown when
			 * `PmergeMe::sort(const T &, T &)`
			 * fails to get the time.
			 * @class	GetTimeFail
			 */
			class GetTimeFail: public std::exception
			{
				private:
					std::string msg;

				public:
					GetTimeFail(const char * msg);
					GetTimeFail(const std::string & msg);
					GetTimeFail(const GetTimeFail & src);
					GetTimeFail & operator = (
							const GetTimeFail & src);
					virtual ~GetTimeFail() throw();

					virtual const char * what() const throw();
			};

			/**
			 * Sort \p IN while saving results in \p out.
			 * @warning	Type stored in \t T
			 * 		must have `operator < ()`
			 * 		overloaded for it
			 * 		and be @c CopyConstructible.
			 * 		if type stored in \t T is one with
			 * 		floating-point, it's up to you
			 * 		to ensure `operator == ()`
			 * 		is precise enough.
			 * @warning	\t T must have `operator == ()`
			 * 		overloaded for it.
			 * @warning	If \p out isn't empty,
			 * 		it will be cleared.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @throw	GetTimeFail	clock_gettime() failed.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `push_back()`,
			 * 			`erase()`, and `size()` methods,
			 * 			`const_iterator`,
			 * 			and `iterator` to
			 * 			iterate through it,
			 * 			and `begin()` method
			 * 			to obtain
			 * 			such iterators.
			 * @param	IN	Container to sort.
			 * @param	out	Result of sorting \p in.
			 * @return	Time taken for the sorting procedure,
			 * 		which is a difference between
			 * 		end time and start time.
			 */
			template <typename T>
			struct timediff sort(const T & IN, T & out)
			{
				// `IN.size() / 2` because
				// there will be `IN.size() / 2` pairs.
				//const size_t PAIRS_SIZE = IN.size() / 2;
				const std::string MSG_PREFIX = "PmergeMe::sort(): ";
				const std::string GET_TIME_FAIL_MSG = "clock_gettime() failed.";
				struct timespec begin_tm, end_tm;
				struct timediff ret;

				this->comp_cnt = 0;
				// "Forbidden functions: none",
				// therefore a system call to get precise time
				// in nanoseconds must be allowed.
				if (clock_gettime(CLOCK_MONOTONIC, &begin_tm)
					== -1)
				{
					throw GetTimeFail(MSG_PREFIX
							+ GET_TIME_FAIL_MSG);
				}
				out = this->merge_insert(IN);
				if (clock_gettime(CLOCK_MONOTONIC, &end_tm)
					== -1)
				{
					throw GetTimeFail(MSG_PREFIX
							+ GET_TIME_FAIL_MSG);
				}
				ret.tv_sec = end_tm.tv_sec - begin_tm.tv_sec;
				ret.tv_nsec = end_tm.tv_nsec - begin_tm.tv_nsec;
				return ret;
			}

			/**
			 * Getter for `comp_cnt`.
			 * @return	`comp_cnt`.
			 */
			size_t get_comparison_count() const;
	};
}

#endif	// PMERGEME_HPP
