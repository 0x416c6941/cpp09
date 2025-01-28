#pragma once

#include <string>
#include <stdexcept>
#include <ostream>
#include <Bureaucrat.hpp>

class Bureaucrat;

class AForm {
    private:
        const std::string m_name;

        bool m_signed;

        static const int m_grade_highest = 1, m_grade_lowest = 150;
        const int m_sign_grade, m_exec_grade;

        // As previously, we don't want default constructor be accessible.
        AForm();

    protected:
        class GradeTooHighException : public std::exception {
            private:
                const char * m_msg;

            public:
                GradeTooHighException(const char * msg);

                const char * what() const throw();
        };

        class GradeTooLowException : public std::exception {
            private:
                const char * m_msg;

            public:
                GradeTooLowException(const char * msg);

                const char * what() const throw();
        };

        class FormNotSignedException : public std::exception {
            private:
                const char * m_msg;

            public:
                FormNotSignedException(const char * msg);

                const char * what() const throw();
        };

    public:
        AForm(const std::string & name, int sign_grade, int exec_grade);
        AForm(const AForm & src);
        AForm & operator = (const AForm & src);
        virtual ~AForm();

        const std::string & get_name() const;
        bool is_signed() const;
        int get_sign_grade() const;
        int get_exec_grade() const;

        void beSigned(const Bureaucrat & by);

        /**
         * Check, if form is signed already and if \p executor has sufficient grade.
         * @param   executor                Executor to check.
         * @throws  FormNotSignedException  Form wasn't signed yet.
         * @throws  GradeTooLowException    If \p executor's grade is unsufficient.
         */
        void check_execute(const Bureaucrat & executor) const;
        /**
         * Try to execute the form. If some execution condition isn't met,
         * throws same exceptions as check_execute().
         * @param   executor    Bureaucrat, that will execute the form.
         * @throws  Please consult with \ref check_execute().
         */
        virtual void execute(const Bureaucrat & executor) const = 0;
};

std::ostream & operator << (std::ostream & os, const AForm & obj);
