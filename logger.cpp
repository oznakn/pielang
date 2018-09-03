#include "logger.h"

#include <iostream>

#include "termcolor.hpp"

void Logger::debug(string ctx, string s) {
    cout << termcolor::bold << termcolor::cyan  << "[" + ctx + "] " << termcolor::reset << termcolor::blue << s << termcolor::reset << endl;
}

void Logger::print(string s) {
    cout << s;
}

void Logger::println(string s) {
    cout << s << endl;
}