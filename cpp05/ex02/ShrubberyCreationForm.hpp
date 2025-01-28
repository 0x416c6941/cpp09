#pragma once

#include <AForm.hpp>
#include <string>
#include <Bureaucrat.hpp>

class ShrubberyCreationForm : public AForm {
    private:
        static const int m_required_sign_grade = 145, m_required_exec_grade = 137;

        std::string m_target;

        ShrubberyCreationForm();

    public:
        ShrubberyCreationForm(const std::string & target);
        ShrubberyCreationForm(const ShrubberyCreationForm & src);
        ShrubberyCreationForm & operator = (const ShrubberyCreationForm & src);
        virtual ~ShrubberyCreationForm();

        virtual void execute(const Bureaucrat & executor) const;
};
