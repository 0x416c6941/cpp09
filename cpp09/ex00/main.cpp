#include <BitcoinExchange.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main(int argc, char ** argv) {
    BitcoinExchange btc_exchange;
    std::ifstream in_file;

    in_file.open("data.csv");
    if (!in_file.is_open()) {
        std::cerr << "main(): data.csv file isn't present." << std::endl;
        return EXIT_FAILURE;
    }
    try {
        btc_exchange.load_exchange_history(in_file);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    (void)argc;
    (void)argv;
    return 0;
}
