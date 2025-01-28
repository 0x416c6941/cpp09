#pragma once

#include <string>
#include <stdexcept>
#include <ostream>
#include <Bureaucrat.hpp>

// Wdym "‘Bureaucrat’ does not name a type"?????
class Bureaucrat;

class Form {
    private:
        const std::string m_name;

        bool m_signed;

        static const int m_grade_highest = 1, m_grade_lowest = 150;
        const int m_sign_grade, m_exec_grade;

        // As previously, we don't want default constructor be accessible.
        Form();

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

    public:
        Form(const std::string & name, int sign_grade, int exec_grade);
        Form(const Form & src);
        Form & operator = (const Form & src);
        virtual ~Form();

        const std::string & get_name() const;
        bool is_signed() const;
        int get_sign_grade() const;
        int get_exec_grade() const;

        void beSigned(const Bureaucrat & by);
};

std::ostream & operator << (std::ostream & os, const Form & obj);
