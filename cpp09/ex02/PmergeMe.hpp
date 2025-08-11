#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <ctime>

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

			// Count of element's comparisons.
			size_t comp_cnt;

			/**
			 * Compare function to sort containers of type \t T
			 * by second element stored in them.
			 * @warning	Type stored in \t T
			 * 		must have `operator < ()`
			 * 		overloaded for it.
			 * @tparam	T	A container type
			 * 			that provides `size()` method,
			 * 			`const_iterator` to
			 * 			iterate through it,
			 * 			and `begin()` method
			 * 			to obtain such iterator.
			 * @class	CompareBySecond
			 */
			template <typename T>
			struct CompareBySecond
			{
				bool operator () (const T & lhs, const T & rhs) const
				{
					typename T::const_iterator lhs_second;
					typename T::const_iterator rhs_second;

					if (lhs.size() < 2 || rhs.size() < 2)
					{
						return false;
					}
					lhs_second = lhs.begin();
					std::advance(lhs_second, 1);
					rhs_second = rhs.begin();
					std::advance(rhs_second, 1);
					return (*lhs_second < *rhs_second);
				}
			};

			/**
			 * Split \p IN to pairs and sort content of each pair.
			 * If element count in \p IN is odd,
			 * last element will remain untouched.
			 * @brief	"Merge" phase of Ford-Johnson algorithm.
			 * @warning	Type stored in \t T
			 * 		must have `operator < ()`
			 * 		overloaded for it
			 * 		and be @c CopyConstructible.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `push_back()`,
			 * 			`erase()`, and `size()` methods,
			 * 			`const_iterator` to
			 * 			iterate through it,
			 * 			and `begin()`, and `end()`
			 * 			methods to obtain
			 * 			such iterators.
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
					if (*it < *next_it)
					{
						ret[i].push_back(*it);
						ret[i].push_back(*next_it);
					}
					else
					{
						ret[i].push_back(*next_it);
						ret[i].push_back(*it);
					}
					this->comp_cnt++;
				}
				return ret;
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
			 * @warning	If \p out isn't empty,
			 * 		it will be cleared.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @tparam	T	A container type that
			 * 			saves the order
			 * 			of inserted elements,
			 * 			provides `push_back()`,
			 * 			`erase()`, and `size()` methods,
			 * 			`const_iterator` to
			 * 			iterate through it,
			 * 			and `begin()`, and `end()`
			 * 			methods to obtain
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
				T * pairs;
				struct timediff ret;

				// "Forbidden functions: none",
				// therefore a system call to get precise time
				// in nanoseconds must be allowed.
				if (clock_gettime(CLOCK_MONOTONIC, &begin_tm)
					== -1)
				{
					throw GetTimeFail(MSG_PREFIX
							+ GET_TIME_FAIL_MSG);
				}
				pairs = this->get_pairs_with_sorted_content(IN);
				delete [] pairs;
				if (clock_gettime(CLOCK_MONOTONIC, &end_tm)
					== -1)
				{
					throw GetTimeFail(MSG_PREFIX
							+ GET_TIME_FAIL_MSG);
				}
				ret.tv_sec = end_tm.tv_sec - begin_tm.tv_sec;
				ret.tv_nsec = end_tm.tv_nsec - begin_tm.tv_nsec;
				(void) out;
				return ret;
			}
	};
}

#endif	// PMERGEME_HPP
