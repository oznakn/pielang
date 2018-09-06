#ifndef PILANG_UTILS_H
#define PILANG_UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    static size_t findSiblingPosition(std::string, size_t, char, char);
    static size_t findStringInVector(std::vector<std::string>*, std::string);
};

#endif //PILANG_UTILS_H
