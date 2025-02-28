#include <easyfind.hpp>
#include <algorithm>
#include <stdexcept>

// We can also use a deep copy of container and not reference to it.
template <typename T>
typename T::iterator easyfind(T & container, int value) {
    typename T::iterator it = std::find(container.begin(), container.end(), value);

    if (it == container.end()) {
        throw std::runtime_error("easyfind(): value not found in container");
    }
    return it;
}
