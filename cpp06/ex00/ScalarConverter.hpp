#pragma once

#include <string>

class ScalarConverter {
    private:
        ScalarConverter();
        ScalarConverter(const ScalarConverter & src);
        ScalarConverter & operator = (const ScalarConverter & src);

        enum e_type {
            INVALID_INPUT,
            USUAL,
            NEGATIVE_INFF,
            POSITIVE_INFF,
            NEGATIVE_INF,
            POSITIVE_INF,
            NAN_LITERAL
            // "", "0d", etc. Please note that input "42." is considered valid.
        };

        union u_value {
            // It's not guaranteed by standard if char is signed or not.
            // It's also not mentioned in the subject, which signedness we need to use,
            // so we'll just use unsigned char.
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

        struct s_value_with_overflow_check {
            union u_value value;
            enum e_data_type data_type;
            bool overflow;
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
         * @param   scalar  String possibly containing a pseudo literal
         *                  with spaces trimmed from beginning and end of the string.
         * @param   type    Output parameter.
         */
        static void set_type(const std::string & scalar, enum e_type & type);

        /**
         * Converts an initial part of \p scalar to character.
         * @warning \p scalar must contain at least 1 character.
         * @param   scalar  String with a scalar with spaces
         *                  trimmed from beginning and end of the string.
         * @return  struct s_value_with_overflow_check with "value" containing a character,
         *              "data_type" set to CHARACTER and "overflow" pointing
         *              if conversion overflows or not.
         */
        static struct s_value_with_overflow_check weird_atoc(const std::string & scalar);

        /**
         * Converts an initial part of \p scalar to integer.
         * @warning \p scalar must contain at least 1 character.
         * @param   scalar  String with a scalar with spaces
         *                  trimmed from beginning and end of the string.
         * @return  struct s_value_with_overflow_check with "value" containing an integer,
         *              "data_type" set to INTEGER and "overflow" pointing
         *              if conversion overflows or not.
         */
        static struct s_value_with_overflow_check weird_atoi(const std::string & scalar);

        /**
         * Converts an initial part of \p scalar to float.
         * @warning \p scalar must contain at least 1 character.
         * @param   scalar  String with a scalar with spaces
         *                  trimmed from beginning and end of the string.
         * @return  struct s_value_with_overflow_check with "value" containing a floating number,
         *              "data_type" set to FLOAT and "overflow" pointing
         *              if conversion overflows or not.
         */
        static struct s_value_with_overflow_check weird_atof(const std::string & scalar);

        /**
         * Converts an initial part of \p scalar to double.
         * @warning \p scalar must contain at least 1 character.
         * @param   scalar  String with a scalar with spaces
         *                  trimmed from beginning and end of the string.
         * @return  struct s_value_with_overflow_check with "value" containing a double,
         *              "data_type" set to DOUBLE and "overflow" pointing
         *              if conversion overflows or not.
         */
        static struct s_value_with_overflow_check weird_atod(const std::string & scalar);

        // Precision of numbers with floating points for std::setprecision().
        static const int FLOAT_POINTS = 1;

    public:
        virtual ~ScalarConverter();

        static void convert(const std::string & scalar);
};
