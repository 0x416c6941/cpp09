#include <ScalarConverter.hpp>
#include <string>
#include <cstddef>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <iomanip>

ScalarConverter::~ScalarConverter() {
}

bool ScalarConverter::is_input_valid(const std::string & scalar) {
    std::size_t i;
    bool got_sign;
    bool got_dot;
    bool got_f;

    if (scalar.length() == 0) {
        return false;
    }
    else if (scalar.length() == 1
            || scalar.compare("-inff") == 0 || scalar.compare("+inff") == 0
            || scalar.compare("-inf") == 0 || scalar.compare("+inf") == 0
            || scalar.compare("nan") == 0) {
        return true;
    }
    got_sign = false;
    got_dot = false;
    got_f = false;
    for (i = 0; i < scalar.length(); i++) {
        if (scalar.at(i) == '+' || scalar.at(i) == '-') {
            if (got_sign) {
                return false;
            }
            got_sign = true;
        }
        else if (scalar.at(i) == '.') {
            if (got_dot) {
                return false;
            }
            got_dot = true;
        }
        else if (scalar.at(i) == 'f') {
            if (got_f) {
                return false;
            }
            got_f = true;
        }
        else if (!std::isdigit(scalar.at(i))) {
            return false;
        }
        if (std::isdigit(scalar.at(i)) && got_f) {
            return false;
        }
    }
    if (!(i >= scalar.length())) {
        return false;
    }
    return true;
}

void ScalarConverter::set_type(const std::string & scalar, enum e_scalar_type & type) {
    if (!is_input_valid(scalar)) {
        type = INVALID_INPUT;
    }
    else if (scalar.compare("-inff") == 0) {
        type = NEGATIVE_INFF;
    }
    else if (scalar.compare("+inff") == 0) {
        type = POSITIVE_INFF;
    }
    else if (scalar.compare("-inf") == 0) {
        type = NEGATIVE_INF;
    }
    else if (scalar.compare("+inf") == 0) {
        type = POSITIVE_INF;
    }
    else if (scalar.compare("nan") == 0) {
        type = NAN_LITERAL;
    }
}

ScalarConverter::s_value_with_check ScalarConverter::handle_conversion(const std::string & scalar,
                                                                       enum e_data_type data_type) {
    struct s_value_with_check ret;
    char *endptr;                   // For stdtol(), ...

    // A lot of duplicate code, but I don't care.
    if (data_type == INTEGER) {
        long val;

        ret.data_type = INTEGER;
        errno = 0;
        val = strtol(scalar.c_str(), &endptr, 10);
        if (scalar.at(0) != '\0' && *endptr == '\0'
            && errno == 0
            && (val >= std::numeric_limits<int>::min()
                && val <= std::numeric_limits<int>::max())) {
            ret.valid = true;
        }
        else {
            ret.valid = false;
        }
        ret.value.i = static_cast<int>(val);
        if (ret.valid == false && scalar.size() == 1) {
            ret.value.i = scalar.at(0);
            ret.valid = true;
        }
    }
    else if (data_type == CHARACTER) {
        long val;

        ret.data_type = CHARACTER;
        errno = 0;
        val = strtol(scalar.c_str(), &endptr, 10);
        if (scalar.at(0) != '\0' && *endptr == '\0' && errno == 0
            && (val >= std::numeric_limits<unsigned char>::min()
                && val <= std::numeric_limits<unsigned char>::max())) {
            ret.valid = true;
        }
        else {
            ret.valid = false;
        }
        ret.value.c = static_cast<unsigned char>(val);
        if (ret.valid == false && scalar.size() == 1
            && scalar.at(0) >= std::numeric_limits<unsigned char>::min()
            && scalar.at(0) <= std::numeric_limits<unsigned char>::max()) {
            ret.value.c = static_cast<unsigned char>(scalar.at(0));
            ret.valid = true;
        }
    }
    else if (data_type == DOUBLE) {
        double val;

        ret.data_type = DOUBLE;
        errno = 0;
        val = strtod(scalar.c_str(), &endptr);
        if (scalar.at(0) != '\0' && *endptr == '\0' && errno == 0) {
            ret.valid = true;
        }
        else {
            ret.valid = false;
        }
        ret.value.d = val;
        if (ret.valid == false && scalar.size() == 1) {
            ret.value.d = scalar.at(0);
            ret.valid = true;
        }
    }
    else if (data_type == FLOAT) {
        float val;

        ret.data_type = FLOAT;
        errno = 0;
        val = strtof(scalar.c_str(), &endptr);
        if (scalar.at(0) != '\0' && *endptr == '\0' && errno == 0) {
            ret.valid = true;
        }
        else {
            ret.valid = false;
        }
        ret.value.f = val;
        if (ret.valid == false && scalar.size() == 1) {
            ret.value.f = scalar.at(0);
            ret.valid = true;
        }
    }
    else {
        throw std::invalid_argument("ScalarConverter::handle_conversion(): Invalid data type");
    }
    return ret;
}

// Please note that a string like "  " is NOT a scalar.
// It's multiple scalars (' ', ' ').
void ScalarConverter::convert(const std::string & scalar) {
    enum e_scalar_type type = USUAL;
    struct s_value_with_check i, c, d, f;

    if (scalar.empty() || !is_input_valid(scalar)) {
        std::cout << "char: impossible" << std::endl
                  << "int: impossible" << std::endl
                  << "float: impossible" << std::endl
                  << "double: impossible" << std::endl;
        return;
    }
    i = handle_conversion(scalar, INTEGER);
    c = handle_conversion(scalar, CHARACTER);
    d = handle_conversion(scalar, DOUBLE);
    f = handle_conversion(scalar, FLOAT);
    std::cout << "char: ";
    if (type != USUAL || !c.valid) {
        std::cout << "impossible" << std::endl;
    }
    else if (!std::isprint(c.value.c)) {
        std::cout << "Non displayable" << std::endl;
    }
    else {
        std::cout << '\'' << c.value.c << '\'' << std::endl;
    }
    std::cout << "int: ";
    if (type != USUAL || !i.valid) {
        std::cout << "impossible" << std::endl;
    }
    else {
        std::cout << i.value.i << std::endl;
    }
    if (type == NEGATIVE_INFF || type == NEGATIVE_INF) {
        std::cout << "float: -inff" << std::endl
                  << "double: -inf" << std::endl;
    }
    else if (type == POSITIVE_INFF || type == POSITIVE_INF) {
        std::cout << "float: +inff" << std::endl
                  << "double: +inf" << std::endl;
    }
    else if (type == NAN_LITERAL) {
        std::cout << "float: nanf" << std::endl
                  << "double: nan" << std::endl;
    }
    else if (type == INVALID_INPUT) {
        std::cout << "float: impossible" << std::endl
                  << "double: impossible" << std::endl;
    }
    else {
        if (!f.valid) {
            std::cout << "float: impossible" << std::endl;
        }
        else {
            std::cout << "float: " << std::fixed << std::setprecision(FLOAT_POINTS) << f.value.f << 'f' << std::endl;
        }
        if (!d.valid) {
            std::cout << "double: impossible" << std::endl;
        }
        else {
            std::cout << "double: " << std::fixed << std::setprecision(FLOAT_POINTS) << d.value.d << std::endl;
        }
    }
}
