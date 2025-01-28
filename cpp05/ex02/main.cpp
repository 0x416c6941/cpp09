#include <Bureaucrat.hpp>
#include <AForm.hpp>
#include <stdexcept>
#include <iostream>

int main() {
    Bureaucrat good_bureaucrat("Some good bureaucrat", 1);
    Bureaucrat bad_bureaucrat("Some bad bureaucrat", 150);
    Form some_form("Good form", 100, 100);

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
    try {
        Form some_form("Nice form", 500, 500);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        some_form.beSigned(bad_bureaucrat);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    some_form.beSigned(good_bureaucrat);
    good_bureaucrat.signForm(some_form);
    std::cout << some_form << std::endl;
    return 0;
}
