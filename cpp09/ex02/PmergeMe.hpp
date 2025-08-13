#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <fstream>
#include <string>

/**
 * Namespace dedicated specifically to PmergeMe to potentially
 * avoid name collision w/ other functions and classes.
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
	 * Parse a single \p arg to a \t T of positive integers.
	 * @warning	If type stored in \t T isn't `int`,
	 * 		the correct behavior of this function
	 * 		isn't guaranteed.
	 * @tparam	T	Container, that saves the order
	 * 			of inserted elements
	 * 			and provides `push_back()`
	 * 			and `empty()` methods.
	 * @throw	std::bad_alloc		Some allocation failed.
	 * @throw	std::invalid_argument	Received an invalid input.
	 * @param	arg	One of arguments from @c argv.
	 * @return	\t T of parsed positive integers from \p arg
	 * 		with the order of elements preserved.
	 */
	template <typename T>
	static T parse_arg(char * arg);

	/**
	 * Parse \p argv of \p argc elements to a \t T of positive integers.
	 * @warning	If type stored in \t T isn't `int`,
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
	 * @return	\t T of parsed positive integers from \p argv
	 * 		with the order of elements preserved.
	 */
	template <typename T>
	T parse_argv(int argc, char * argv[]);

	/**
	 * Print container with beginning iterator \p begin
	 * and ending iterator \p end to \p os.
	 * @warning	Type stored in container must have
	 * 		`operator << ()` overloaded for it.
	 * @tparam	Iterator	Iterator of a container
	 * 				to print content of on `stdout`.
	 * @param	os		File based stream to write
	 * 				container content to.
	 * @param	begin		Iterator of type \t T,
	 * 				which is a beginning of that container.
	 * @param	end		Iterator of type \t T,
	 * 				which is an ending of that container.
	 */
	template <class Iterator>
	void print_container(Iterator begin, Iterator end);

	/**
	 * Write \p TMDIFF to \p os.
	 * @param	os	File based stream to write \p TMDIFF to.
	 * @param	TMDIFF	Time diff to print to \p os.
	 */
	void print_timediff(
			std::ofstream & os, const struct timediff & TMDIFF);

	/**
	 * An implementation of merge-insertion sort
	 * (Ford-Johnson algorithm).
	 * @warning	If type stored in \t Sort_Container
	 * 		isn't `int`, the correct behavior
	 * 		of this class isn't guaranteed.
	 * @tparam	Sort_Container	A container type that preserves
	 * 				the order of inserted elements,
	 * 				provides `insert()`, `push_back()`,
	 * 				`erase()` and `size()` methods,
	 * 				`const_iterator` and `iterator` to
	 * 				iterate through it
	 * 				and `begin()` and `end()` methods
	 * 				to obtain such iterators.
	 * 				It must have default constructor
	 * 				accessible and be @c CopyConstructible
	 * 				and @c CopyAssignable.
	 * @class	PmergeMe
	 */
	template <typename Sort_Container>
	class PmergeMe
	{
		private:
			// It doesn't make sense for
			// `PmergeMe<Sort_Container>` to be
			// CopyConstructible or CopyAssignable.
			PmergeMe(const PmergeMe & src);
			PmergeMe & operator = (const PmergeMe & src);

			Sort_Container data;

		public:
			PmergeMe();
			virtual ~PmergeMe();

			/**
			 * An exception that's thrown when
			 * `PmergeMe<Sort_Container>::sort(const T &)`
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
			 * Sort \p WHAT and save result to `data`.
			 * @warning	Type stored in \t T
			 * 		must be `int`,
			 * 		otherwise the correct behavior
			 * 		of this method is not guaranteed.
			 * @warning	If `data` already stores
			 * 		any information, it will be discarded.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @throw	GetTimeFail	clock_gettime() failed.
			 * @tparam	T	A container type
			 * 			that preserves the order
			 * 			of inserted elements,
			 * 			provides `size()` method,
			 * 			`const_iterator`
			 * 			to iterate through it
			 * 			and `begin()` and `end()`
			 * 			methods to obtain it.
			 * @param	WHAT	Container with data to sort.
			 * @return	Time taken to sort \p WHAT.
			 */
			template <typename T>
			struct timediff sort(const T & WHAT);

			/**
			 * Getter for `data`.
			 * @return	`data`.
			 */
			const Sort_Container & get_result() const;
	};
}

// TODO: Change the files's name below to "PmergeMe.tpp".
#include "_PmergeMe.cpp"

#endif	// PMERGEME_HPP
