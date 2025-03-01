#pragma once

#include <map>
#include <string>

class BitcoinExchange {
    private:
        struct Date {
            int year;
            short month;
            short day;
        };

        std::map<struct Date, unsigned> m_exchange_history;
        bool m_history_loaded;

        /**
         * Constructs "Date" from the date stored in \p date.
         * It must be in "YY-MM-DD" format.
         * @warning Each string not in the format provided above
         *          will be treated as invalid, including
         *          strings like "2022-01-01 ".
         * @param   date    Date.
         * @throw   std::invalid_argument if \p date contains invalid date.
         * @return  Constructed date.
         */
        struct Date ctor_date(const std::string & date) const;

        /**
         * Validates if \p header of a database file is valid.
         * @param   header  Database files header.
         * @return  true, if header is valid; false, if header is not valid.
         */
        bool validate_db_header(const std::string & header) const;

    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange & src);
        BitcoinExchange & operator = (const BitcoinExchange & src);
        virtual ~BitcoinExchange();

        /**
         * Loads the exchange history from \p db_file.
         * @param   db_file Database file.
         * @throw   std::ios_base::failure  \p db_file is corrupted.
         */
        void load_exchange_history(std::ifstream & db_file);

        /**
         * Does the exchange of all bitcoins in \p in_file
         * and provides the output to stdout and stderr.
         * @param   in_file Input file.
         */
        void exchange(std::ifstream & in_file) const;
};
