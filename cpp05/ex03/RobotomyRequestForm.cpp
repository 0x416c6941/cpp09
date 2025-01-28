#include <RobotomyRequestForm.hpp>
#include <string>
#include <AForm.hpp>
#include <Bureaucrat.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

RobotomyRequestForm::RobotomyRequestForm(const std::string & target)
    : AForm("Some RobotomyRequestForm", m_required_sign_grade, m_required_exec_grade),
      m_target(target) {
}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm & src)
    : AForm(src), m_target(src.m_target) {
}

RobotomyRequestForm & RobotomyRequestForm::operator = (const RobotomyRequestForm & src) {
    if (this == &src) {
        return *this;
    }
    AForm::operator = (src);
    m_target = src.m_target;
    return *this;
}

RobotomyRequestForm::~RobotomyRequestForm() {
}

void RobotomyRequestForm::execute(const Bureaucrat & executor) const {
    check_execute(executor);
    srand(time(NULL));
    if (rand() % 2 != 1) {
        std::cout << "Robotomy failed. Bad luck :(" << std::endl;
        return;
    }
    std::cout << "Robotomy succeeded on ";
    if (m_target.empty()) {
        std::cout << "unnamed target";
    }
    else {
        std::cout << m_target;
    }
    std::cout << '.' << std::endl;
}
