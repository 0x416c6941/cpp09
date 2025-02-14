#pragma once

// "The first one has type T".
// Well, then we can also just set the type to "T &", right?
template <typename T>
typename T::iterator easyfind(T & container, int value);

#include <easyfind.tpp>
