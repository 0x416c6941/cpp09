#include <Data.hpp>
#include <Serializer.hpp>
#include <iostream>

int main() {
    Data data("Hello there!");

    std::cout << reinterpret_cast<const char *>(Serializer::deserialize(Serializer::serialize(&data))->get_data()) << std::endl;
    return 0;
}
