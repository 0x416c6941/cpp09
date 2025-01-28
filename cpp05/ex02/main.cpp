#include <Bureaucrat.hpp>
#include <AForm.hpp>
#include <ShrubberyCreationForm.hpp>
#include <stdexcept>
#include <iostream>

int main() {
    const int SHRUBBERY_CREATION_FORM_EXEC_GRADE = 137;
    Bureaucrat quite_good_bureaucrat("Some good bureaucrat",
                                     SHRUBBERY_CREATION_FORM_EXEC_GRADE + 1);
    AForm *some_form = new ShrubberyCreationForm("ohyeaaaah");

    quite_good_bureaucrat.signForm(*some_form);
    std::cout << *some_form << std::endl;
    quite_good_bureaucrat.executeForm(*some_form);
    quite_good_bureaucrat.increment_grade();
    quite_good_bureaucrat.executeForm(*some_form);
    delete some_form;
    return 0;
}
