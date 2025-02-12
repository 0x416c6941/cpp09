#pragma once

#include <vector>

class Span {
    private:
        std::vector<int> m_vec;
        unsigned int m_vec_max;

        Span();

    public:
        Span(unsigned int N);
        Span(const Span & src);
        Span & operator = (const Span & src);
        virtual ~Span();

        void addNumber(int number);
        template <typename Iterator>
        void addNumber(Iterator begin, Iterator end);

        int shortestSpan() const;
        int longestSpan() const;
};

#include <Span.tpp>
