#include <Base.hpp>
#include <ctime>
#include <cstdlib>
#include <A.hpp>
#include <B.hpp>
#include <C.hpp>
#include <iostream>
#include <stdexcept>

#define CLASSES 3

Base * generate(void) {

    srand(time(NULL));
    switch (rand() % CLASSES) {
        case 0:
            return new A();
        case 1:
            return new B();
        case 2:
            return new C();
        default:
            return NULL;    // If this happened, then something is really weird.
    }
}

void identify(Base * p) {
    if (dynamic_cast<A *>(p) != NULL) {
        std::cout << 'A';
    }
    else if (dynamic_cast<B *>(p) != NULL) {
        std::cout << 'B';
    }
    else if (dynamic_cast<C *>(p) != NULL) {
        std::cout << 'C';
    }
    else {
        std::cout << "identify(): Something went wrong.";
    }
}

void identify(Base & p) {
    try {
        dynamic_cast<A &>(p);
        std::cout << 'A';
        return;
    }
    catch (std::exception & e) {
    }
    try {
        dynamic_cast<B &>(p);
        std::cout << 'B';
        return;
    }
    catch (std::exception & e) {
    }
    try {
        dynamic_cast<C &>(p);
        std::cout << 'C';
        return;
    }
    catch (std::exception & e) {
    }
    std::cout << "identify(): Something went wrong.";
}

int main(void) {
    Base * obj = generate();

    identify(obj);
    std::cout << std::endl;
    identify(*obj);
    std::cout << std::endl;
    delete obj;
    return 0;
}
