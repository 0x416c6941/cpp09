#pragma once

#include <AForm.hpp>
#include <string>

class Intern {
    private:
        // It doesn't make sense to have those two,
        // as Intern doesn't have any fields.
        Intern(const Intern & src);
        Intern & operator = (const Intern & src);

        AForm * create_shrubbery_creation_form(const std::string & target) const;
        AForm * create_robotomy_request_form(const std::string & target) const;
        AForm * create_presidential_pardon_form(const std::string & target) const;

    public:
        Intern();
        virtual ~Intern();

        AForm * makeForm(const std::string & type, const std::string & target);
};
