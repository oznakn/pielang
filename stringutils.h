#ifndef PILANG_STRINGUTILS_H
#define PILANG_STRINGUTILS_H

#include <string>
#include <vector>

using namespace std;

class StringUtils {
public:
    static bool contains(string, string);
    static size_t count(string, char);
    static size_t count(string, string);
    static string substring(string, size_t, size_t);
    static string cutMiddle(string, size_t, size_t);
    static string replaceMiddle(string, string, size_t, size_t);
    static string ltrim(string);
    static string rtrim(string);
    static string trim(string);
    static vector<string>* split(string, string);
};


#endif //PILANG_STRINGUTILS_H
