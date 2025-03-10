#include <MutantStack.hpp>
#include <iostream>
#include <stack>
#include <list>

int main() {
    MutantStack<int> mstack;
    std::stack<int> test_assignment_op;

    mstack.push(5);
    mstack.push(17);
    std::cout << mstack.top() << std::endl;
    mstack.pop();
    std::cout << mstack.size() << std::endl;
    mstack.push(3);
    mstack.push(5);
    mstack.push(737);
    //[...]
    mstack.push(0);
    MutantStack<int>::iterator it = mstack.begin();
    MutantStack<int>::iterator ite = mstack.end();
    ++it;
    --it;
    while (it != ite) {
        std::cout << *it << std::endl;
        ++it;
    }
    test_assignment_op = mstack;
    std::cout << "mstack.size(): " << mstack.size()
              << ", test_assignment_op.size(): " << test_assignment_op.size() << std::endl;
    return 0;
}
