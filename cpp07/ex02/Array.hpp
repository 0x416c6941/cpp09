#pragma once

#include <cstddef>

template <typename T>
class Array {
    private:
        T * m_arr;
        std::size_t m_arr_size;

    public:
        Array(unsigned int n = 0);
        Array(const Array<T> & src);
        Array<T> & operator = (const Array<T> & src);
        virtual ~Array();

        T & operator [] (int idx);
        std::size_t size() const;
};

#include <Array.tpp>
