#ifndef PIELANG_LOGGER_H
#define PIELANG_LOGGER_H

#include <string>
#include <ctime>
#include <unordered_map>

#include "definitions.h"

class Logger {
private:
    static std::unordered_map<std::string, clock_t>* timeMap;

public:
    static void debug(std::string, std::string);
    static void print(std::string);
    static void println(std::string);

    static void startTimer(std::string);
    static void endTimer(std::string);
};


#endif //PIELANG_LOGGER_H
