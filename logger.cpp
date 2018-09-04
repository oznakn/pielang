#include "logger.h"

#include <iostream>

#include "termcolor.hpp"

void Logger::debug(std::string ctx, std::string s) {
    std::cout << termcolor::bold << termcolor::cyan  << "[" + ctx + "] " << termcolor::reset << termcolor::blue << s << termcolor::reset << std::endl;
}

void Logger::print(std::string s) {
    std::cout << s;
}

void Logger::println(std::string s) {
    std::cout << s << std::endl;
}