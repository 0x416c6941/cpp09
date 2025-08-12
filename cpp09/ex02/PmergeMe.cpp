#include "PmergeMe.hpp"
#include <string>
#include <cstddef>

PmergeMe::PmergeMe::PmergeMe()
{
}

/*
PmergeMe::PmergeMe::PmergeMe(const PmergeMe & src)
{
	(void) src;
}

PmergeMe::PmergeMe & PmergeMe::PmergeMe::operator = (const PmergeMe & src)
{
	if (this == &src)
	{
		return *this;
	}
	return *this;
}
 */

PmergeMe::PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe::InvalidPairSize::InvalidPairSize(const char * msg)
	: msg(msg)
{
}

PmergeMe::PmergeMe::InvalidPairSize::InvalidPairSize(const std::string & msg)
	: msg(msg)
{
}

PmergeMe::PmergeMe::InvalidPairSize::InvalidPairSize(
		const PmergeMe::PmergeMe::InvalidPairSize & src)
	: msg(src.msg)
{
}

PmergeMe::PmergeMe::InvalidPairSize & PmergeMe::PmergeMe::InvalidPairSize:: operator = (
		const PmergeMe::PmergeMe::InvalidPairSize & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

PmergeMe::PmergeMe::InvalidPairSize::~InvalidPairSize() throw()
{
}

const char * PmergeMe::PmergeMe::InvalidPairSize::what() const throw()
{
	return this->msg.c_str();
}

PmergeMe::PmergeMe::GetTimeFail::GetTimeFail(const char * msg)
	: msg(msg)
{
}

PmergeMe::PmergeMe::GetTimeFail::GetTimeFail(const std::string & msg)
	: msg(msg)
{
}

PmergeMe::PmergeMe::GetTimeFail::GetTimeFail(
		const PmergeMe::PmergeMe::GetTimeFail & src)
	: msg(src.msg)
{
}

PmergeMe::PmergeMe::GetTimeFail & PmergeMe::PmergeMe::GetTimeFail:: operator = (
		const PmergeMe::PmergeMe::GetTimeFail & src)
{
	if (this == &src)
	{
		return *this;
	}
	this->msg = src.msg;
	return *this;
}

PmergeMe::PmergeMe::GetTimeFail::~GetTimeFail() throw()
{
}

const char * PmergeMe::PmergeMe::GetTimeFail::what() const throw()
{
	return this->msg.c_str();
}
