#pragma once

#include <AForm.hpp>
#include <string>
#include <Bureaucrat.hpp>

class RobotomyRequestForm : public AForm {
    private:
        static const int m_required_sign_grade = 72, m_required_exec_grade = 45;

        std::string m_target;
        // It doesn't make sense to proceed with robotomy multiple times.
        bool m_executed;

        RobotomyRequestForm();

        class RobotomyAlreadyExecutedException: public std::exception {
            private:
                const char * m_msg;

            public:
                RobotomyAlreadyExecutedException(const char * msg);

                const char * what() const throw();
        };

    public:
        RobotomyRequestForm(const std::string & target);
        RobotomyRequestForm(const RobotomyRequestForm & src);
        RobotomyRequestForm & operator = (const RobotomyRequestForm & src);
        virtual ~RobotomyRequestForm();

        virtual void execute(const Bureaucrat & executor) const;
};
