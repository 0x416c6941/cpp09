#pragma once

#include <cstddef>

template<typename T, typename F>
void iter(T * arr, std::size_t len, F func) {
    for(std::size_t i = 0; i < len; i++) {
        func(arr[i]);
    }
}
