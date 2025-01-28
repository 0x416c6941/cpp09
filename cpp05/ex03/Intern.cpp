#include <Intern.hpp>
#include <AForm.hpp>
#include <string>
#include <ShrubberyCreationForm.hpp>
#include <RobotomyRequestForm.hpp>
#include <PresidentialPardonForm.hpp>
#include <iostream>
#include <cctype>

/**
 * Transforms \s to be lowercase.
 * @param   s   String to transform.
 * @return  \p s transformed to lowercase.
 */
static std::string string_tolower(const std::string & s);

Intern::Intern() {
}

Intern::~Intern() {
}

AForm * Intern::create_shrubbery_creation_form(const std::string & target) const {
    return new ShrubberyCreationForm(target);
}

AForm * Intern::create_robotomy_request_form(const std::string & target) const {
    return new RobotomyRequestForm(target);
}

AForm * Intern::create_presidential_pardon_form(const std::string & target) const {
    return new PresidentialPardonForm(target);
}

#define TYPES   3

AForm * Intern::makeForm(const std::string & type, const std::string & target) {
    const std::string types[TYPES] = { "shrubbery creation", "robotomy request", "presidential pardon" };
    AForm * (Intern::* create_funcs[TYPES])(const std::string & target) const
        = { &Intern::create_shrubbery_creation_form, &Intern::create_robotomy_request_form, &Intern::create_presidential_pardon_form };
    std::string type_lowercase = string_tolower(type);

    for (size_t i = 0; i < TYPES; i++) {
        if (type_lowercase.compare(types[i]) == 0) {
            std::cout << "Intern creates " << type_lowercase << std::endl;
            return (this->*create_funcs[i])(target);
        }
    }
    // "If the form name passed as parameter doesn’t exist, print an explicit error message."
    // So this is good, right?... I don't want to create another exception type :(
    std::cerr << "Intern::makeForm(): Form of the provided type doesn't exist" << std::endl;
    return NULL;
}

static std::string string_tolower(const std::string & s) {
    std::string ret;
    size_t i = 0;

    while (i < s.length()) {
        ret.push_back(std::tolower(s.at(i)));
        i++;
    }
    return ret;
}
