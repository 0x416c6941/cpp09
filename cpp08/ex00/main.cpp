#include <easyfind.hpp>
#include <vector>
#include <iostream>

#define VEC_ELEMS   10

int main() {
    std::vector<int> vec;
    std::vector<int>::iterator it;

    for (int i = 0; i < VEC_ELEMS; i++) {
        vec.push_back(i);
    }
    try {
        it = easyfind(vec, 1337);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    it = easyfind(vec, 5);
    std::cout << *it << std::endl;
    return 0;
}
