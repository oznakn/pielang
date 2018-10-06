#ifndef PILANG_LOGGER_H
#define PILANG_LOGGER_H

#include <string>
#include <ctime>
#include <unordered_map>

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


#endif //PILANG_LOGGER_H
