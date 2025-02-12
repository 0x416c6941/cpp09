#include <Span.hpp>
#include <stdexcept>
#include <algorithm>

template <typename Iterator>
void Span::addNumber(Iterator begin, Iterator end) {
    unsigned int count = std::distance(begin, end);

    if (m_vec.size() + count > m_vec_max) {
        throw std::runtime_error("Span::addNumber(): Too many elements");
    }
    m_vec.insert(m_vec.end(), begin, end);
}
