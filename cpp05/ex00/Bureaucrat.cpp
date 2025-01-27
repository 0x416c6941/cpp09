#include <Bureaucrat.hpp>
#include <string>

Bureaucrat::Bureaucrat(const std::string & name, int grade) : m_name(name) {
    if (grade < m_GRADE_HIGHEST) {
        throw GradeTooHighException("Bureaucrat::Bureaucrat(): Grade is too high");
    }
    else if (grade > m_GRADE_LOWEST) {
        throw GradeTooLowException("Bureaucrat::Bureaucrat(): Grade is too low");
    }
    m_grade = grade;
}

Bureaucrat::Bureaucrat(const Bureaucrat & src) : m_name(src.m_name) {
    if (src.m_grade < m_GRADE_HIGHEST) {
        throw GradeTooHighException("Bureaucrat::Bureaucrat(): Grade is too high");
    }
    else if (src.m_grade > m_GRADE_LOWEST) {
        throw GradeTooLowException("Bureaucrat::Bureaucrat(): Grade is too low");
    }
    m_grade = src.m_grade;
}

Bureaucrat & Bureaucrat::operator = (const Bureaucrat & src) {
    if (this == &src) {
        return *this;
    }
    if (src.m_grade < m_GRADE_HIGHEST) {
        throw GradeTooHighException("Bureaucrat::operator = (): Grade is too high");
    }
    else if (src.m_grade > m_GRADE_LOWEST) {
        throw GradeTooLowException("Bureaucrat::operator = (): Grade is too low");
    }
    m_grade = src.m_grade;
    return *this;
}

Bureaucrat::~Bureaucrat() {
}

Bureaucrat::GradeTooHighException::GradeTooHighException(const char * msg)
    : m_msg(msg) {
}

const char * Bureaucrat::GradeTooHighException::what() const throw() {
    return m_msg;
}

Bureaucrat::GradeTooLowException::GradeTooLowException(const char * msg)
    : m_msg(msg) {
}

const char * Bureaucrat::GradeTooLowException::what() const throw() {
    return m_msg;
}

const std::string & Bureaucrat::getName() const {
    return m_name;
}

int Bureaucrat::getGrade() const {
    return m_grade;
}

void Bureaucrat::increment_grade() {
    if (m_grade - 1 < m_GRADE_HIGHEST) {
        throw GradeTooHighException("Bureaucrat::increment_grade(): Grade is already the highest possible");
    }
    m_grade--;
}

void Bureaucrat::decrement_grade() {
    if (m_grade + 1 > m_GRADE_LOWEST) {
        throw GradeTooHighException("Bureaucrat::decrement_grade(): Grade is already the highest possible");
    }
    m_grade++;
}

std::ostream & operator << (std::ostream & os, const Bureaucrat & obj) {
    if (obj.getName().empty()) {
        os << "Unnamed bureaucrat";
    }
    else {
        os << obj.getName();
    }
    os << ", bureaucrat grade " << obj.getGrade();
    return os;
}
