#include <Form.hpp>
#include <string>
#include <ostream>
#include <Bureaucrat.hpp>

Form::Form(const std::string & name, int sign_grade, int exec_grade)
    : m_name(name), m_signed(false), m_sign_grade(sign_grade), m_exec_grade(exec_grade) {
    if (sign_grade < m_grade_highest || exec_grade < m_grade_highest) {
        throw GradeTooHighException("Form::Form(): Some grade is too high");
    }
    else if (sign_grade > m_grade_lowest || exec_grade > m_grade_lowest) {
        throw GradeTooLowException("Form::Form(): Some grade is too low");
    }
}

Form::Form(const Form & src)
    : m_name(src.m_name), m_sign_grade(src.m_sign_grade), m_exec_grade(src.m_exec_grade) {
    if (src.m_sign_grade < m_grade_highest || src.m_exec_grade < m_grade_highest) {
        throw GradeTooHighException("Form::Form(): Some grade is too high");
    }
    else if (src.m_sign_grade > m_grade_lowest || src.m_exec_grade > m_grade_lowest) {
        throw GradeTooLowException("Form::Form(): Some grade is too low");
    }
}

Form & Form::operator = (const Form & src) {
    if (this == &src) {
        return *this;
    }
    if (src.m_sign_grade < m_grade_highest || src.m_exec_grade < m_grade_highest) {
        throw GradeTooHighException("Form::operator = (): Some grade is too high");
    }
    else if (src.m_sign_grade > m_grade_lowest || src.m_exec_grade > m_grade_lowest) {
        throw GradeTooLowException("Form::operator = (): Some grade is too low");
    }
    return *this;
}

Form::~Form() {
}

Form::GradeTooHighException::GradeTooHighException(const char * msg) : m_msg(msg) {
}

const char * Form::GradeTooHighException::what() const throw() {
    return m_msg;
}

Form::GradeTooLowException::GradeTooLowException(const char * msg) : m_msg(msg) {
}

const char * Form::GradeTooLowException::what() const throw() {
    return m_msg;
}

const std::string & Form::get_name() const {
    return m_name;
}

bool Form::is_signed() const {
    return m_signed;
}

int Form::get_sign_grade() const {
    return m_sign_grade;
}

int Form::get_exec_grade() const {
    return m_exec_grade;
}

void Form::beSigned(const Bureaucrat & by) {
    if (m_sign_grade < by.getGrade()) {
        throw GradeTooLowException("Form::beSigned(): Bureaucrat's grade is too low");
    }
    /* We don't care if it was signed already or not, subject doesn't specify
     * what we have to do in this case. */
    m_signed = true;
}

std::ostream & operator << (std::ostream & os, const Form & obj) {
    if (obj.get_name().empty()) {
        os << "Unnamed form";
    }
    else {
        os << "Form \"" << obj.get_name() << '\"';
    }
    os << ", is_signed(): " << obj.is_signed()
       << ", get_sign_grade(): " << obj.get_sign_grade()
       << ", get_exec_grade(): " << obj.get_exec_grade();
    return os;
}
