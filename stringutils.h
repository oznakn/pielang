#ifndef PILANG_STRINGUTILS_H
#define PILANG_STRINGUTILS_H

#include <string>
#include <vector>

class StringUtils {
public:
    static bool isSpaceLike(char);
    static bool isNotSpaceLike(char);
    static bool isBothSpaceLike(char, char);
    static bool isBothNotSpaceLike(char, char);
    static bool contains(std::string, std::string);
    static size_t count(std::string, char);
    static size_t count(std::string, std::string);
    static std::string substring(std::string, size_t, size_t);
    static std::string cutMiddle(std::string, size_t, size_t);
    static std::string replaceMiddle(std::string, std::string, size_t, size_t);
    static std::string ltrim(std::string);
    static std::string rtrim(std::string);
    static std::string trim(std::string);
    static std::vector<std::string>* split(std::string, std::string);
    static std::string replaceMultipleSpacesWithOne(std::string);
    static std::string replaceMultipleCharsWithOne(std::string, char c);
    static std::string duplicateWithCount(std::string, size_t);
};


#endif //PILANG_STRINGUTILS_H
