#include <ShrubberyCreationForm.hpp>
#include <string>
#include <AForm.hpp>
#include <Bureaucrat.hpp>
#include <fstream>
#include <stdexcept>

ShrubberyCreationForm::ShrubberyCreationForm(const std::string & name, const std::string & target)
    : AForm(name, m_required_sign_grade, m_required_exec_grade),
      m_target(target) {
}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm & src)
    : AForm(src), m_target(src.m_target) {
}

ShrubberyCreationForm & ShrubberyCreationForm::operator = (const ShrubberyCreationForm & src) {
    if (this == &src) {
        return *this;
    }
    AForm::operator = (src);
    m_target = src.m_target;
    return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {
}

void ShrubberyCreationForm::execute(const Bureaucrat & executor) const {
    std::ofstream output;

    check_execute(executor);
    output.open(std::string(m_target).append("_shrubbery").c_str(), std::ofstream::trunc);
    if (!output.is_open()) {
        throw std::ios_base::failure("ShrubberyCreationForm::execute(): Couldn't open file");
    }
    output << "Let's" << std::endl
           << "imagine" << std::endl
           << "this" << std::endl
           << "is" << std::endl
           << "a" << std::endl
           << "tree" << std::endl;
    if (!output.good()) {
        throw std::ios_base::failure("ShrubberyCreationForm::execute(): Got an error during writing");
    }
}
