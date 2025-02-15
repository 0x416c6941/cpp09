#include <MutantStack.hpp>
#include <vector>
#include <stdexcept>
#include <cstddef>

template <typename T>
MutantStack<T>::MutantStack(): m_data() {
}

template <typename T>
MutantStack<T>::MutantStack(const MutantStack<T> & src) : m_data(src.m_data) {
}

template <typename T>
MutantStack<T> & MutantStack<T>::operator = (const MutantStack<T> & src) {
    if (this == &src) {
        return *this;
    }
    m_data = src.m_data;
    return *this;
}

template <typename T>
MutantStack<T>::~MutantStack() {
}

template <typename T>
T & MutantStack<T>::top() {
    // Behavior is undefined, we don't really have to do this.
    if (m_data.empty()) {
        throw std::runtime_error("MutantStack::pop(): Stack is empty");
    }
    return m_data.back();
}

template <typename T>
const T & MutantStack<T>::top() const {
    // Behavior is undefined, we don't really have to do this.
    if (m_data.empty()) {
        throw std::runtime_error("MutantStack::pop(): Stack is empty");
    }
    return m_data.back();
}

template <typename T>
bool MutantStack<T>::empty() const {
    return m_data.empty();
}

template <typename T>
std::size_t MutantStack<T>::size() const {
    return m_data.size();
}

template <typename T>
void MutantStack<T>::push(const T & value) {
    m_data.push_back(value);
}

template <typename T>
void MutantStack<T>::pop() {
    // Behavior is undefined, we don't really have to do this.
    if (m_data.empty()) {
        throw std::runtime_error("MutantStack::pop(): Stack is empty");
    }
    m_data.pop_back();
}
