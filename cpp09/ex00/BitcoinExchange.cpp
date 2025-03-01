#include <BitcoinExchange.hpp>
#include <string>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange() : m_exchange_history(), m_history_loaded(false) {
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange & src)
    : m_exchange_history(src.m_exchange_history), m_history_loaded(src.m_history_loaded) {
}

BitcoinExchange & BitcoinExchange::operator = (const BitcoinExchange & src) {
    if (this == &src) {
        return *this;
    }
    m_exchange_history = src.m_exchange_history;
    m_history_loaded = src.m_history_loaded;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {
}

BitcoinExchange::Date BitcoinExchange::ctor_date(const std::string & date) const {
    struct Date ret;
    std::stringstream ss(date);
    std::string in;
    long num;
    char * strtol_endptr;
    const int STRTOL_BASE = 10;
    const short MAX_POSSIBLE_MONTH = 12, MAX_POSSIBLE_DAY = 31;

    std::getline(ss, in, '-');
    if (!ss.good()) {
        throw std::invalid_argument("BitcoinExchange::ctor_date(): Date is invalid");
    }
    num = strtol(in.c_str(), &strtol_endptr, STRTOL_BASE);
    if (*strtol_endptr != '\0'
        || num < std::numeric_limits<int>::min() || num > std::numeric_limits<int>::max()) {
        throw std::invalid_argument("BitcoinExchange::ctor_date(): Date is invalid");
    }
    ret.year = static_cast<int> (num);
    std::getline(ss, in, '-');
    if (!ss.good()) {
        throw std::invalid_argument("BitcoinExchange::ctor_date(): Date is invalid");
    }
    num = strtol(in.c_str(), &strtol_endptr, STRTOL_BASE);
    if (*strtol_endptr != '\0'
        || num < 1 || num > MAX_POSSIBLE_MONTH) {
        throw std::invalid_argument("BitcoinExchange::ctor_date(): Date is invalid");
    }
    ret.month = static_cast<short> (num);
    std::getline(ss, in);
    if (ss.fail() || ss.bad()) {
        throw std::invalid_argument("BitcoinExchange::ctor_date(): Date is invalid");
    }
    num = strtol(in.c_str(), &strtol_endptr, STRTOL_BASE);
    // It's not the point of the exercise to check if e.g.
    // it's not a leap year and february contains more than 28 days,
    // so we'll just treat every date with day not bigger than 31 as valid.
    if (*strtol_endptr != '\0'
        || num < 1 || num > MAX_POSSIBLE_DAY) {
        throw std::invalid_argument("BitcoinExchange::ctor_date(): Date is invalid");
    }
    ret.month = static_cast<short> (num);
    return ret;
}

bool BitcoinExchange::validate_db_header(const std::string & header) const {
    std::stringstream ss(header);
    std::string in;

    std::getline(ss, in, ',');
    // We assume the first column in the database is "date", and second is "exchange_rate".
    // We could, of course, make it also possible to be them in order "exchange_rate,date",
    // but doing so is not the point of this exercise...
    // ... And, frankly, I don't care.
    if (!ss.good() || in.compare("date") != 0) {
        return false;
    }
    std::getline(ss, in, ',');
    if ((ss.fail() || ss.bad()) || in.compare("exchange_rate") != 0) {
        return false;
    }
    ss >> in;
    if (!ss.eof()) {
        return false;
    }
    return true;
}

void BitcoinExchange::load_exchange_history(std::ifstream & db_file) {
    std::string header;

    std::getline(db_file, header);
    if (!db_file.good() || !validate_db_header(header)) {
        throw std::ios_base::failure("BitcoinExchange()::BitcoinExchange(): Database is corrupted");
    }
    m_history_loaded = true;
}
