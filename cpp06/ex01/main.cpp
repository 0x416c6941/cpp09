#include <Data.hpp>
#include <Serializer.hpp>
#include <iostream>

int main(void) {
    Data data("Hello there!");

    std::cout << (const char *) Serializer::deserialize(Serializer::serialize(&data)) << std::endl;
    return 0;
}
