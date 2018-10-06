#include "logger.h"

#include <iostream>
#include <ctime>
#include <unordered_map>

#include "termcolor.hpp"

std::unordered_map<std::string, clock_t>* Logger::timeMap = new std::unordered_map<std::string, clock_t>;

void Logger::debug(std::string ctx, std::string s) {
    std::cout << termcolor::bold << termcolor::cyan  << "[" + ctx + "] " << termcolor::reset << termcolor::blue << s << termcolor::reset << std::endl;
}

void Logger::print(std::string s) {
    std::cout << s;
}

void Logger::println(std::string s) {
    std::cout << s << std::endl;
}

void Logger::startTimer(std::string ctx) {
    Logger::timeMap->insert(std::make_pair(ctx, clock()));
}

void Logger::endTimer(std::string ctx) {
    clock_t start = Logger::timeMap->at(ctx);
    clock_t end = clock();
    Logger::timeMap->erase(ctx);

    std::cout << termcolor::bold << termcolor::blue << ctx << ": " << termcolor::reset << termcolor::cyan << ((double(end - start)/ CLOCKS_PER_SEC) * 1000) << "ms" << termcolor::reset << std::endl;
}