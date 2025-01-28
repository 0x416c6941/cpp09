#include <Intern.hpp>
#include <AForm.hpp>

int main() {
    Intern some_guy;
    AForm * form;

    form = some_guy.makeForm("robotomy request", "Bender");
    delete form;
    return 0;
}
