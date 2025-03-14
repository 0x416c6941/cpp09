#pragma once

template <typename T>
void swap(T & lhs, T & rhs) {
    T tmp = lhs;

    lhs = rhs;
    rhs = tmp;
}

template <typename T>
T min(T lhs, T rhs) {
    if (lhs < rhs) {
        return lhs;
    }
    return rhs;
}

template <typename T>
T max(T lhs, T rhs) {
    if (lhs > rhs) {
        return lhs;
    }
    return rhs;
}
