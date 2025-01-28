#include <PresidentialPardonForm.hpp>
#include <string>
#include <AForm.hpp>
#include <Bureaucrat.hpp>
#include <iostream>

PresidentialPardonForm::PresidentialPardonForm(const std::string & name, const std::string & target)
    : AForm(name, m_required_sign_grade, m_required_exec_grade),
      m_target(target) {
}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm & src)
    : AForm(src), m_target(src.m_target) {
}

PresidentialPardonForm & PresidentialPardonForm::operator = (const PresidentialPardonForm & src) {
    if (this == &src) {
        return *this;
    }
    AForm::operator = (src);
    m_target = src.m_target;
    return *this;
}

PresidentialPardonForm::~PresidentialPardonForm() {
}

void PresidentialPardonForm::execute(const Bureaucrat & executor) const {
    check_execute(executor);
    if (m_target.empty()) {
        std::cout << "unnamed target";
    }
    else {
        std::cout << m_target;
    }
    std::cout << " has been pardoned by Zaphod Beeblebrox." << std::endl;
}
