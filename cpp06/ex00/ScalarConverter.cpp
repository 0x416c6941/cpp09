#include <ScalarConverter.hpp>
#include <string>
#include <cstddef>
#include <cctype>
#include <climits>
#include <cfloat>
#include <iostream>
#include <iomanip>

/**
 * Returns a copy of \p s with spaces trimmed from the beginning and end.
 * @param   s   String to trim spaces from the beginning and end from.
 * @return  Copy of \p s with spaces trimmed from the beginning and end.
 */
static std::string trim_spaces(const std::string & s);

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

void ScalarConverter::set_type(const std::string & scalar, enum e_type & type) {
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

ScalarConverter::s_value_with_overflow_check ScalarConverter::weird_atoc(const std::string & scalar) {
    struct s_value_with_overflow_check ret = weird_atoi(scalar);

    if (ret.value.i < 0 || ret.value.i > UCHAR_MAX) {
        ret.overflow = true;
    }
    ret.value.c = static_cast<unsigned char>(ret.value.i);
    ret.data_type = CHARACTER;
    return ret;
}

ScalarConverter::s_value_with_overflow_check ScalarConverter::weird_atoi(const std::string & scalar) {
    struct s_value_with_overflow_check ret;
    int sign;
    bool at_least_one_digit;
    std::size_t i;

    ret.value.i = 0;
    ret.data_type = INTEGER;
    ret.overflow = false;
    if (scalar.length() == 1 && !std::isdigit(scalar.at(0))) {
        ret.value.i = scalar.at(0);
        return ret;
    }
    sign = 1;
    at_least_one_digit = false;
    i = 0;
    if (scalar.at(i) == '+' || scalar.at(i) == '-') {
        if (scalar.at(i) == '-') {
            sign = -1;
        }
        i++;
    }
    for (; i < scalar.length(); i++) {
        if (!std::isdigit(scalar.at(i))) {
            break;
        }
        at_least_one_digit = true;
        if ((ret.value.i * 10 + (scalar.at(i) - '0')) < ret.value.i) {
            ret.overflow = true;
            break;
        }
        ret.value.i = ret.value.i * 10 + (scalar.at(i) - '0');
    }
    if (sign == -1) {
        ret.value.i *= -1;
    }
    if (!at_least_one_digit) {
        ret.overflow = true;    // Not an overflow, just to show an error.
    }
    return ret;
}

ScalarConverter::s_value_with_overflow_check ScalarConverter::weird_atof(const std::string & scalar) {
    struct s_value_with_overflow_check ret = weird_atod(scalar);

    // Trying to compare with FLT_MIN would probably need to use DBL_EPSILON.
    // I'm too lazy to figure out this stuff :p
    if (ret.value.d > FLT_MAX) {
        ret.overflow = true;
    }
    ret.value.f = static_cast<float>(ret.value.d);
    ret.data_type = FLOAT;
    return ret;
}

ScalarConverter::s_value_with_overflow_check ScalarConverter::weird_atod(const std::string & scalar) {
    struct s_value_with_overflow_check ret;
    int sign;
    bool at_least_one_digit;
    bool got_dot;
    std::size_t i;
    std::size_t j;

    ret.value.d = 0;
    ret.data_type = DOUBLE;
    ret.overflow = false;
    if (scalar.length() == 1 && !std::isdigit(scalar.at(0))) {
        ret.value.d = scalar.at(0);
        return ret;
    }
    sign = 1;
    at_least_one_digit = false;
    got_dot = false;
    i = 0;
    if (scalar.at(i) == '+' || scalar.at(i) == '-') {
        if (scalar.at(i) == '-') {
            sign = -1;
        }
        i++;
    }
    for (; i < scalar.length(); i++) {
        if (scalar.at(i) == 'f') {
            break;
        }
        if (std::isdigit(scalar.at(i))) {
            at_least_one_digit = true;
            if (!got_dot) {
                if ((ret.value.d * 10 + (scalar.at(i) - '0')) < ret.value.d) {
                    ret.overflow = true;
                    break;
                }
                ret.value.d = ret.value.d * 10 + (scalar.at(i) - '0');
            }
            else {
                double to_add = scalar.at(i) - '0';
                for (std::size_t k = 0; k < j; k++) {
                    to_add /= 10;
                }
                ret.value.d += to_add;
                j++;
            }
        }
        else if (scalar.at(i) == '.') {
            if (got_dot) {
                ret.overflow = true;    // Not an overflow, just to show an error.
                break;
            }
            got_dot = true;
            j = 1;
        }
        else {
            ret.overflow = true;    // Not an overflow, just to show an error.
            break;
        }
    }
    if (sign == -1) {
        ret.value.d *= -1;
    }
    if (!at_least_one_digit) {
        ret.overflow = true;    // Not an overflow, just to show an error.
    }
    return ret;
}

void ScalarConverter::convert(const std::string & scalar) {
    std::string scalar_trimmed_spaces = trim_spaces(scalar);
    enum e_type type = USUAL;
    struct s_value_with_overflow_check c = weird_atoc(scalar_trimmed_spaces),
                                       i = weird_atoi(scalar_trimmed_spaces),
                                       f = weird_atof(scalar_trimmed_spaces),
                                       d = weird_atod(scalar_trimmed_spaces);

    set_type(scalar_trimmed_spaces, type);
    std::cout << "char: ";
    if (type != USUAL || c.overflow) {
        std::cout << "impossible" << std::endl;
    }
    else if (!std::isprint(c.value.c)) {
        std::cout << "Non displayable" << std::endl;
    }
    else {
        std::cout << '\'' << c.value.c << '\'' << std::endl;
    }
    std::cout << "int: ";
    if (type != USUAL || i.overflow) {
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
        if (f.overflow) {
            std::cout << "float: impossible" << std::endl;
        }
        else {
            std::cout << "float: " << std::fixed << std::setprecision(FLOAT_POINTS) << f.value.f << 'f' << std::endl;
        }
        if (d.overflow) {
            std::cout << "double: impossible" << std::endl;
        }
        else {
            std::cout << "double: " << std::fixed << std::setprecision(FLOAT_POINTS) << d.value.d << std::endl;
        }
    }
}

static std::string trim_spaces(const std::string & s) {
    std::string ret = s;

    while (ret.length() != 0 && ret.at(0) == ' ') {
        ret.erase(0, 1);
    }
    while (ret.length() != 0 && ret.at(ret.length() - 1) == ' ') {
        ret.erase(ret.length() - 1, 1);
    }
    return ret;
}
