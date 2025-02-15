#pragma once

#include <vector>
#include <cstddef>
#include <stack>

template <typename T>
class MutantStack : public std::stack<T> {
    private:
        std::vector<T> m_data;

    public:
        MutantStack();
        MutantStack(const MutantStack<T> & src);
        MutantStack<T> & operator = (const MutantStack<T> & src);
        virtual ~MutantStack();

        // Calling top() on empty stack is undefined behavior.
        T & top();
        const T & top() const;

        bool empty() const;
        std::size_t size() const;

        void push(const T & value);
        // Calling pop() on empty stack is undefined behavior.
        void pop();
};

#include <MutantStack.tpp>
