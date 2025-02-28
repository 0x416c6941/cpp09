#include <Array.hpp>
#include <cstddef>
#include <stdexcept>

template <typename T>
Array<T>::Array(unsigned int n) : m_arr_size(n) {
    if (n == 0) {
        m_arr = NULL;
    }
    else {
        m_arr = new T[n]();
    }
}

template <typename T>
Array<T>::Array(const Array<T> & src) : m_arr_size(src.m_arr_size) {
    if (src.m_arr_size == 0) {
        m_arr = NULL;
    }
    else {
        m_arr = new T[src.m_arr_size];
        for (std::size_t i = 0; i < src.m_arr_size; i++) {
            m_arr[i] = src.m_arr[i];
        }
    }
}

template <typename T>
Array<T> & Array<T>::operator = (const Array<T> & src) {
    if (this == &src) {
        return *this;
    }
    delete [] m_arr;
    m_arr_size = src.m_arr_size;
    if (src.m_arr_size == 0) {
        m_arr = NULL;
    }
    else {
        m_arr = new T[src.m_arr_size];
        for (std::size_t i = 0; i < src.m_arr_size; i++) {
            m_arr[i] = src.m_arr[i];
        }
    }
    return *this;
}

template <typename T>
Array<T>::~Array() {
    delete [] m_arr;
}

template <typename T>
T & Array<T>::operator [] (int idx) {
    if (idx < 0 || (std::size_t) idx >= m_arr_size) {
        // I mean it's derived from std::exception, so it should be fine.
        throw std::out_of_range("Array::operator [] (): Index out of bounds");
    }
    return m_arr[idx];
}

template <typename T>
std::size_t Array<T>::size() const {
    return m_arr_size;
}
