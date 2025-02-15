#pragma once

#include <string>

class ScalarConverter {
    private:
        ScalarConverter();
        ScalarConverter(const ScalarConverter & src);
        ScalarConverter & operator = (const ScalarConverter & src);
        virtual ~ScalarConverter();

        enum e_scalar_type {
            INVALID_INPUT,
            USUAL,
            NEGATIVE_INFF,
            POSITIVE_INFF,
            NEGATIVE_INF,
            POSITIVE_INF,
            NAN_F,
            NAN_D
        };

        union u_value {
            // It's not guaranteed by standard if char is signed or not.
            // It's also not mentioned in the subject,
            // which signedness we need to use, so we'll just use unsigned char.
            unsigned char c;
            int i;
            float f;
            double d;
        };

        enum e_data_type {
            CHARACTER,
            INTEGER,
            FLOAT,
            DOUBLE
        };

        struct s_value_with_check {
            union u_value value;
            enum e_data_type data_type;
            bool valid;
        };

        /**
         * Checks if input in \p scalar is valid or not.
         * @param   scalar  String with scalar to check.
         * @return  true, if yes; false, if no.
         */
        static bool is_input_valid(const std::string & scalar);

        /**
         * Depending on the string in \p scalar, sets the \p type
         * of pseudo literal.
         * @param   scalar  String possibly containing a pseudo literal.
         * @param   type    Output parameter.
         */
        static void set_type(const std::string & scalar, enum e_scalar_type & type);

        /**
         * Handle the conversion of \p scalar to \p data_type.
         * @param   scalar      String with a scalar.
         * @param   data_type   To which type to convert the scalar.
         * @return  Converted value with the done validity check.
         */
        static struct s_value_with_check handle_conversion(const std::string & scalar,
                                                           enum e_data_type data_type);

        // Precision of numbers with floating points for std::setprecision().
        static const int FLOAT_POINTS = 1;

    public:
        static void convert(const std::string & scalar);
};
