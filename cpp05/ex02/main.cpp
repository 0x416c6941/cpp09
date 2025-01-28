#include <Bureaucrat.hpp>
#include <AForm.hpp>
#include <ShrubberyCreationForm.hpp>
#include <RobotomyRequestForm.hpp>
#include <stdexcept>
#include <iostream>

int main() {
    const int SHRUBBERY_CREATION_FORM_EXEC_GRADE = 137, BEST_GRADE = 1;
    Bureaucrat quite_good_bureaucrat("Some good bureaucrat",
                                     SHRUBBERY_CREATION_FORM_EXEC_GRADE + 1);
    Bureaucrat best_bureaucrat("THE BEST BUREAUCRAT", BEST_GRADE);
    AForm *some_form = new ShrubberyCreationForm("ohyeaaaah");

    quite_good_bureaucrat.signForm(*some_form);
    std::cout << *some_form << std::endl;
    quite_good_bureaucrat.executeForm(*some_form);
    quite_good_bureaucrat.increment_grade();
    quite_good_bureaucrat.executeForm(*some_form);
    delete some_form;
    some_form = new RobotomyRequestForm("Some object I don't know");
    best_bureaucrat.signForm(*some_form);
    best_bureaucrat.executeForm(*some_form);
    delete some_form;
    return 0;
}
