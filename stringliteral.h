#ifndef PILANG_STRINGLITERAL_H
#define PILANG_STRINGLITERAL_H

#include <string>
#include <vector>

class Value;

class StringLiteral {
private:
    static std::vector<StringLiteral*>* stringLiteralList;

    std::string mString;
    size_t mIndex;

public:
    static bool isValidStringLiteral(std::string);
    static StringLiteral* getStringLiteral(size_t);
    static StringLiteral* findStringLiteral(std::string);

    StringLiteral(std::string);
    Value* getValue();
    size_t getIndex();
    std::string getAsString();
};

#endif //PILANG_STRINGLITERAL_H
