#pragma once

#include <stack>

// By default, std::stack uses std::deque, which has iterator methods available.
template <typename T>
class MutantStack : public std::stack<T> {
    public:
        MutantStack();
        MutantStack(const MutantStack<T> & src);
        MutantStack<T> & operator = (const MutantStack<T> & src);
        virtual ~MutantStack();

        // (C) cppreference.com: container_type -> Container.
        typedef typename std::stack<T>::container_type::iterator iterator;
        typedef typename std::stack<T>::container_type::reverse_iterator reverse_iterator;
        typename std::stack<T>::container_type::iterator begin();
        typename std::stack<T>::container_type::iterator end();
        typename std::stack<T>::container_type::reverse_iterator rbegin();
        typename std::stack<T>::container_type::reverse_iterator rend();
};

#include <MutantStack.tpp>
