#ifndef PILANG_LOGGER_H
#define PILANG_LOGGER_H

#include <string>

using namespace std;

class Logger {
public:
    static void debug(string, string);
    static void print(string);
    static void println(string);
};


#endif //PILANG_LOGGER_H
