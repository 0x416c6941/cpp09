#include <RobotomyRequestForm.hpp>
#include <string>
#include <AForm.hpp>
#include <Bureaucrat.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

RobotomyRequestForm::RobotomyRequestForm(const std::string & target)
    : AForm("Some ShrubberyCreationForm", m_required_sign_grade, m_required_exec_grade),
      m_target(target), m_executed(false) {
}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm & src)
    : AForm(src), m_target(src.m_target), m_executed(src.m_executed) {
}

RobotomyRequestForm & RobotomyRequestForm::operator = (const RobotomyRequestForm & src) {
    if (this == &src) {
        return *this;
    }
    AForm::operator = (src);
    m_target = src.m_target;
    m_executed = src.m_executed;
    return *this;
}

RobotomyRequestForm::~RobotomyRequestForm() {
}

RobotomyRequestForm::RobotomyAlreadyExecutedException::RobotomyAlreadyExecutedException(const char * msg)
    : m_msg(msg) {
}

const char * RobotomyRequestForm::RobotomyAlreadyExecutedException::what() const throw() {
    return m_msg;
}

void RobotomyRequestForm::execute(const Bureaucrat & executor) const {
    if (!check_execute(executor)) {
        if (!is_signed()) {
            throw FormNotSignedException("RobotomyRequestForm::execute(): Form is not signed yet");
        }
        throw FormNotSignedException("RobotomyRequestForm::execute(): Executors grade is too low");
    }
    if (m_executed) {
        throw RobotomyAlreadyExecutedException("RobotomyRequestForm()::execute(): Robotomy was already executed");
    }
    srand(time(NULL));
    if (rand() % 2 != 1) {
        std::cout << "Robotomy failed. Bad luck :(" << std::endl;
        return;
    }
    std::cout << "Robotomy succeeded on " << m_target << std::endl;
}
