#include "RPN.hpp"
#include <string>

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

// TODO: Should this be `long` instead?
double RPN::evaluate(const std::string & expression)
{
	// "You don’t need to handle brackets or decimal numbers",
	// therefore using `int` here.
	std::stack<int> numbers;

	(void) expression;
	(void) numbers;
	return 0;
}
