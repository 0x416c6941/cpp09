#pragma once

#include <AForm.hpp>
#include <string>
#include <Bureaucrat.hpp>

class PresidentialPardonForm : public AForm {
    private:
        static const int m_required_sign_grade = 25, m_required_exec_grade = 5;

        std::string m_target;

        PresidentialPardonForm();

    public:
        PresidentialPardonForm(const std::string & target);
        PresidentialPardonForm(const PresidentialPardonForm & src);
        PresidentialPardonForm & operator = (const PresidentialPardonForm & src);
        virtual ~PresidentialPardonForm();

        virtual void execute(const Bureaucrat & executor) const;
};
