#include <Data.hpp>
#include <cstddef>

Data::Data() : m_data(NULL) {
}

Data::Data(const void * data) : m_data(data) {
}

Data::Data(const Data & src) : m_data(src.m_data) {
}

Data & Data::operator = (const Data & src) {
    if (this == &src) {
        return *this;
    }
    m_data = src.m_data;
    return *this;
}

Data::~Data() {
}

const void * Data::get_data() const {
    return m_data;
}

void Data::set_data(const void * data) {
    m_data = data;
}
