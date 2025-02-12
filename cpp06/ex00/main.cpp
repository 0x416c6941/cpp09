#include <ScalarConverter.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./ex00 <scalar>" << std::endl;
        return EXIT_FAILURE;
    }
    ScalarConverter::convert(std::string(*(++argv)));
    return 0;
}
