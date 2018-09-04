#ifndef PILANG_LOGGER_H
#define PILANG_LOGGER_H

#include <string>

class Logger {
public:
    static void debug(std::string, std::string);
    static void print(std::string);
    static void println(std::string);
};


#endif //PILANG_LOGGER_H
