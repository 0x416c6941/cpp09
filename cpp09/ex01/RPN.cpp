#include "RPN.hpp"
#include <string>
#include <stack>
#include <cerrno>
#include <cstdlib>
#include <limits>

/*
RPN::RPN()
{
}

RPN::RPN(const RPN & src)
{
	(void) src;
}
 */

RPN & RPN::operator = (const RPN & src)
{
	if (this == &src)
	{
		return *this;
	}
	return *this;
}

RPN::~RPN()
{
}

RPN::InvalidExpression::InvalidExpression(const char * msg)
	: msg(msg)
{
}

RPN::InvalidExpression::InvalidExpression(const std::string & msg)
	: msg(msg)
{
}


RPN::InvalidExpression::InvalidExpression(const RPN::InvalidExpression & src)
	: msg(src.msg)
{
}

RPN::InvalidExpression & RPN::InvalidExpression::operator = (
		const RPN::InvalidExpression & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

RPN::InvalidExpression::~InvalidExpression() throw()
{
}

const char * RPN::InvalidExpression::what() const throw()
{
	return this->msg.c_str();
}

void RPN::calculate_op(std::stack<int> & numbers, char op)
{
	int first, second, result;
	// We need at least 2 top elements to calculate `op`.
	const int MIN_ELEMENTS = 2;
	const std::string MSG_PREFIX = "RPN::calculate_op(): ";
	const std::string O_OR_U_MSG = "Given operation overflows or underflows.";

	if (numbers.size() < MIN_ELEMENTS)
	{
		throw InvalidExpression(MSG_PREFIX
				+ "Stack doesn't contain at least two numbers.");
	}
	second = numbers.top();
	numbers.pop();
	first = numbers.top();
	numbers.pop();
	switch (op)
	{
		case '+':
			result = first + second;
			if ((first < 0 && second < 0 && result > 0)
				|| (first > 0 && second > 0 && result < 0))
			{
				throw InvalidExpression(MSG_PREFIX + O_OR_U_MSG);
			}
			break;
		case '-':
			result = first - second;
			if ((first > 0 && second < 0 && result < 0)
				|| (first < 0 && second > 0 && result > 0))
			{
				throw InvalidExpression(MSG_PREFIX + O_OR_U_MSG);
			}
			break;
		case '*':
			result = first * second;
			if (second != 0 && result / second != first)
			{
				throw InvalidExpression(MSG_PREFIX + O_OR_U_MSG);
			}
			break;
		case '/':
			if (second == 0)
			{
				throw InvalidExpression(MSG_PREFIX
						+ "Got division by 0.");
			}
			result = first / second;
			break;
		default:
			throw InvalidExpression(MSG_PREFIX
					+ "Given operation isn't valid.");
	}
	numbers.push(result);
}

int RPN::evaluate(const char * expression)
{
	// "You don’t need to handle brackets or decimal numbers",
	// therefore using `int` here.
	std::stack<int> numbers;
	long to_push;
	// "The numbers used in this operation and passed as arguments ..."
	// "... will always be less than 10."
	const int RIGHT_OPEN_BOUNDARY = 10;
	char * expression_pos;
	const int STRTOL_BASE = 10;
	const std::string MSG_PREFIX = "RPN::evaluate(): ";

	while (*expression != '\0')
	{
		if (*expression == '+' || *expression == '-'
			|| *expression == '*' || *expression == '/')
		{
			// `*expression++` to skip the expression immediately
			// after `this->calculate_op()` execution.
			RPN::calculate_op(numbers, *expression++);
		}
		else
		{
			errno = 0;
			to_push = strtol(expression, &expression_pos,
					STRTOL_BASE);
			if ((*expression_pos != ' ' && *expression_pos != '\0')
				|| errno == ERANGE
				|| to_push < std::numeric_limits<int>::min()
				|| to_push > RIGHT_OPEN_BOUNDARY)
			{
				throw InvalidExpression(MSG_PREFIX
						+ "Found illegal number "
						+ " or character.");
			}
			numbers.push(static_cast<int> (to_push));
			expression = expression_pos;
		}
		// Skip ' '.
		if (*expression == ' ')
		{
			expression++;
		}
	}
	if (numbers.size() != 1)
	{
		throw InvalidExpression(MSG_PREFIX
				+ "Given expression doesn't result "
				+ "in exactly 1 number.");
	}
	return numbers.top();
}
