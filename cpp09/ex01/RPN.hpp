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
		static double evaluate(const std::string & expression);
};

#endif	// RPN_HPP
