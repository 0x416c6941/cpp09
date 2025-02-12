#include <iter.hpp>
#include <iostream>

void print_line_with_int(int i) {
    std::cout << i << std::endl;
}

int main() {
    int objs[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    iter(objs, sizeof(objs) / sizeof(*objs), print_line_with_int);
    return 0;
}
