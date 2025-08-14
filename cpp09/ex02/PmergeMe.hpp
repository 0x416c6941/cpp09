#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <ostream>
#include <string>
#include <cstddef>

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
	 * @param	os		Stream to write
	 * 				container content to.
	 * @param	begin		Iterator of type \t T,
	 * 				which is a beginning of that container.
	 * @param	end		Iterator of type \t T,
	 * 				which is an ending of that container.
	 */
	template <class Iterator>
	void print_container(
			std::ostream & os, Iterator begin, Iterator end);

	/**
	 * Write \p TMDIFF to \p os.
	 * @param	os	Stream to write \p TMDIFF to.
	 * @param	TMDIFF	Time diff to print to \p os.
	 */
	void print_timediff(
			std::ostream & os, const struct timediff & TMDIFF);

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
	 * 				It must also have default constructor
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

			/**
			 * Where to store the sorted sequence of numbers.
			 * @var	data
			 */
			Sort_Container data;

			/**
			 * "Jacobsthal numbers".
			 * Used to determine the insertion sequence
			 * of elements from "pend" to "main chain".
			 * Note: program memory consumption
			 * and overall executione time
			 * may be greatly decreased,
			 * if we just use `(2^n - (-1)^n) / 3` formula,
			 * where n is number of Jacobsthal number
			 * (source of formula: Wikipedia).
			 * @var	jn
			 */
			Sort_Container jn;

			/**
			 * Generate \p a and \p from pairs of \p SRC.
			 * \p a and \p b in this case are the same pools
			 * as described in merge-insertion sort.
			 * Each element in \p a and \p b is a pair:
			 * the first is element itself,
			 * the second is index in \p SRC.
			 * If `SRC.size()` is odd,
			 * last element remains untouched.
			 * @warning	If \p a or \p b point
			 * 		to an allocated heap memory
			 * 		upon method calling,
			 * 		memory leak would occur.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @param	SRC	Container with some elements.
			 * @param	a	Pairs of smaller elements
			 * 			from pairs of \p SRC
			 * 			and their indices in \p SRC.
			 * @param	b	Pairs of bigger elements
			 * 			from pairs of \p SRC
			 * 			and their indices in \p SRC.
			 * @return	Size of both \p a and \p b.
			 */
			std::size_t populate_a_and_b(
					const Sort_Container & SRC,
					Sort_Container * & a,
					Sort_Container * & b);

			/**
			 * Move \p old_a and \p old_b
			 * to \p new_a and \p new_b respectively
			 * in the order given by \p NEW_ORDER.
			 * @warning	\p old_a and \p old_b
			 * 		must be valid pointers.
			 * @warning	It's up to you to ensure
			 * 		both \p old_a and \p old_b
			 * 		are of the same size as \p NEW_ORDER
			 * 		before calling this method.
			 * @warning	It's up to you to ensure
			 * 		all elements in \p old_a and \p old_b
			 * 		are valid pairs used in
			 * 		merge-insertion sort.
			 * @warning	If \p new_a or \p new_b point
			 * 		to an allocated heap memory
			 * 		upon method calling,
			 * 		memory leak would occur.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @param	NEW_ORDER	New order of pairs
			 * 				in both \p old_a
			 * 				and \p old_b.
			 * @param	old_a		`a` pool.
			 * @param	old_b		`b` pool.
			 * @param	new_a		Where to move
			 * 				\p old_a in an order
			 * 				given by \p NEW_ORDER.
			 * @param	new_b		Where to move
			 * 				\p old_b in an order
			 * 				given by \p NEW_ORDER.
			 */
			void reorder_a_and_b(const Sort_Container NEW_ORDER,
					Sort_Container * & old_a,
					Sort_Container * & old_b,
					Sort_Container * & new_a,
					Sort_Container * & new_b);

			/**
			 * Populates \p main_chain with first element of \p a
			 * and all elements of \p b.
			 * Each container in \p main_chain will be
			 * a pair of two elements:
			 * first is a number from original sequence of numbers,
			 * second is it's index in that same sequence.
			 * @warning	\p a and \p b
			 * 		must be valid pointers.
			 * @warning	It's up to you to ensure
			 * 		both \p a and \p b
			 * 		are of the same size
			 * 		of \p A_AND_B_SIZE.
			 * @warning	It's up to you to ensure
			 * 		all elements in \p a and \p b
			 * 		are valid pairs used in
			 * 		merge-insertion sort.
			 * @warning	If \p main_chain points
			 * 		to an allocated heap memory
			 * 		upon method calling,
			 * 		memory leak would occur.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @param	main_chain	Main chain to allocate
			 * 				of size
			 * 				\p MAIN_CHAIN_SIZE
			 * 				and populate with
			 * 				first element
			 * 				from \p a
			 * 				and all elements
			 * 				from \p b.
			 * @param	MAIN_CHAIN_SIZE	Required size
			 * 				of \p main_chain.
			 * @param	a		Lesser elements
			 * 				of pairs from
			 * 				original sequence
			 * 				of numbers.
			 * @param	b		Bigger elements
			 * 				of pairs from
			 * 				original sequence
			 * 				of numbers.
			 * @param	A_AND_B_SIZE	Size of \p and \p b.
			 */
			void populate_main_chain(Sort_Container * & main_chain,
					const std::size_t MAIN_CHAIN_SIZE,
					const Sort_Container * a,
					const Sort_Container * b,
					const std::size_t A_AND_B_SIZE);

			/**
			 * Populate \p pend with second and onwards
			 * lesser elements from \p a
			 * (\p pend would therefore be of size `A_SIZE - 1`).
			 * Each container in \p pend will container
			 * three elements:
			 * first is a number from \p a,
			 * second is it's index in the original
			 * 	sequence of numbers,
			 * third is index of it's bigger element from `b`
			 * in "main chain".
			 * @warning	\p a must be a valid pointers.
			 * @warning	It's up to you to ensure
			 * 		\p a is of the same size
			 * 		of \p A_SIZE.
			 * @warning	It's up to you to ensure
			 * 		all elements in \p a
			 * 		are valid pairs used in
			 * 		merge-insertion sort.
			 * @warning	If \p pend points
			 * 		to an allocated heap memory
			 * 		upon method calling,
			 * 		memory leak would occur.
			 * @param	main_chain	Main chain to allocate
			 * 				of size
			 * 				`\p A_SIZE - 1`
			 * 				and populate with
			 * 				second and onward
			 * 				elements
			 * 				from \p a.
			 * @param	a		Lesser elements
			 * 				of pairs from
			 * 				original sequence
			 * 				of numbers.
			 * @param	A_SIZE		Size of \p a.
			 */
			void populate_pend(Sort_Container * & pend,
					const Sort_Container * a,
					const std::size_t A_SIZE);

			/**
			 * Populate \p insertion_sequence with help
			 * of Jacobsthal numbers.
			 * @warning	If \p insertion_sequence isn't empty,
			 * 		data in it will be discarded.
			 * @param	insertion_sequence	Where to store
			 * 					the insertion
			 * 					sequence.
			 * @param	PEND_SIZE		For how many
			 * 					elements
			 * 					to generate
			 * 					insertion
			 * 					sequence.
			 */
			void populate_insertion_sequence(
					Sort_Container & insertion_sequence,
					const std::size_t PEND_SIZE);

			/**
			 * Get such sequence of indices
			 * of elements of \p TO_SORT,
			 * so that \p TO_SORT would be sorted.
			 * To achieve that, merge-insertion sort
			 * ("Ford-Johnson algorithm") is used.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @param	TO_SORT	Container of elements
			 * 			to generate such
			 * 			sequence of indices,
			 * 			so that is gets sorted.
			 * @return	Seqence of indices of elements
			 * 		in \p TO_SORT,
			 * 		so that \p TO_SORT would get sorted.
			 */
			Sort_Container get_sorted_sequence_indices(
					const Sort_Container & TO_SORT);

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
			 * Sort \p IN and save result to `data`.
			 * @warning	If `data` already stores
			 * 		any information, it will be discarded.
			 * @throw	std::bad_alloc	Some allocation failed.
			 * @throw	GetTimeFail	clock_gettime() failed.
			 * @param	IN	Container with data to sort.
			 * @return	Time taken to sort \p IN.
			 */
			struct timediff sort(const Sort_Container & IN);

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
