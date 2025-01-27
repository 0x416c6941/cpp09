#pragma once    // No "#pragma once" support? Too bad :((

#include <string>
#include <stdexcept>
#include <ostream>

class Bureaucrat {
    private:
        const std::string m_name;

        static const int m_grade_highest = 1, m_grade_lowest = 150;
        int m_grade;

        // We don't want a default constuctor to be accessible.
        // I believe this doesn't disrupt "Orthodox Canonical Form",
        // since even we try to access it, compiler would give us an error.
        Bureaucrat();

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
        Bureaucrat(const std::string & name, int grade);
        Bureaucrat(const Bureaucrat & src);
        Bureaucrat & operator = (const Bureaucrat & src);
        virtual ~Bureaucrat();

        const std::string & getName() const;
        int getGrade() const;

        void increment_grade();
        void decrement_grade();
};

std::ostream & operator << (std::ostream & os, const Bureaucrat & obj);
