#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <stdexcept>
#include <string>
#include <fstream>

/**
 * A static class to evaluate postfix ("Reverse Polish") notation.
 * @class	RPN
 */
class RPN
{
	private:
		RPN();
		RPN(const RPN & src);

		/**
		 * Calculate \p op with top 2 elements from \p numbers,
		 * where the penultimate top element is the first number,
		 * and push the result to the top of \p numbers,
		 * while popping previous 2 elements of the calculation.
		 * @throw	InvalidExpression	\p op is invalid
		 * 					or \p numbers
		 * 					don't contain
		 * 					at least 2 elements.
		 * @param	numbers	Numbers of postfix notation.
		 * @param	op	Operation to calculate.
		 */
		static void calculate_op(std::stack<int> & numbers, char op);

	public:
		// It doesn't matter if we have `operator = ()`
		// overloaded here, since both default and copy constructors
		// are private and therefore unavailable outside of this class.
		RPN & operator = (const RPN & src);
		virtual ~RPN();

		/**
		 * An exception that's thrown when
		 * `RPN::evaluate(const std::string &)`
		 * encounters invalid expression.
		 * @class	InvalidExpression
		 */
		class InvalidExpression: public std::exception
		{
			private:
				std::string msg;

			public:
				InvalidExpression(const char * msg);
				InvalidExpression(const std::string & msg);
				InvalidExpression(const InvalidExpression & src);
				InvalidExpression & operator = (
						const InvalidExpression & src);
				virtual ~InvalidExpression() throw();

				virtual const char * what() const throw();
		};

		/**
		 * Evaluate \p expression.
		 * @throw	InvalidExpression	\p expression
		 * 					is invalid.
		 * @param	expression	Expression to evaluate.
		 * @return	Result of expression evaluation.
		 */
		static int evaluate(const char * expression);
};

#endif	// RPN_HPP
