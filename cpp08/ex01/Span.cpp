#include <Span.hpp>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <cstdlib>

Span::Span(unsigned int N) : m_vec(), m_vec_max(N) {
}

Span::Span(const Span & src) : m_vec(src.m_vec), m_vec_max(src.m_vec_max) {
}

Span & Span::operator = (const Span & src) {
    if (this == &src) {
        return *this;
    }
    m_vec = src.m_vec;
    m_vec_max = src.m_vec_max;
    return *this;
}

Span::~Span() {
}

void Span::addNumber(int number) {
    if (m_vec.size() >= m_vec_max) {
        throw std::runtime_error("Span::addNumber(): Too many elements");
    }
    m_vec.push_back(number);
}

int Span::shortestSpan() const {
    int min = std::numeric_limits<int>::max();

    if (m_vec.size() < 2) {
        throw std::runtime_error("Span::shortestSpan(): Not enough numbers");
    }
    for (size_t i = 1; i < m_vec.size(); i++) {
        min = std::min(min, abs(m_vec.at(i - 1) - m_vec.at(i)));
    }
    return min;
}

int Span::longestSpan() const {
    int max = std::numeric_limits<int>::min();

    if (m_vec.size() == 0) {
        throw std::runtime_error("Span::longestSpan(): Not enough numbers");
    }
    for (size_t i = 1; i < m_vec.size(); i++) {
        max = std::max(max, abs(m_vec.at(i - 1) - m_vec.at(i)));
    }
    return max;
}
