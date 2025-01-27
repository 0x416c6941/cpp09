#include <Bureaucrat.hpp>
#include <stdexcept>
#include <iostream>

int main() {
    Bureaucrat good_bureaucrat("Some good bureaucrat", 1);
    Bureaucrat bad_bureaucrat("Some bad bureaucrat", 150);
    try {
        Bureaucrat mister("Jones", 0);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        good_bureaucrat.increment_grade();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        bad_bureaucrat.decrement_grade();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << good_bureaucrat << std::endl
              << bad_bureaucrat << std::endl;
    return 0;
}
