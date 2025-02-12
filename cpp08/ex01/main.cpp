#include <Span.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>

#define SP_MAX  100

int main() {
    Span sp = Span(SP_MAX);
    std::vector<int> vec;

    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);
    std::cout << sp.shortestSpan() << std::endl;
    std::cout << sp.longestSpan() << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < SP_MAX / 2; i++) {
        vec.push_back(i);
    }
    sp.addNumber(vec.begin(), vec.end());
    std::cout << sp.shortestSpan() << std::endl;
    std::cout << sp.longestSpan() << std::endl;
    std::cout << std::endl;
    try {
        sp.addNumber(vec.begin(), vec.end());
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
