#ifndef PILANG_STRINGLITERAL_H
#define PILANG_STRINGLITERAL_H

#include <string>
#include <vector>

class StringLiteral {
private:
    static std::vector<StringLiteral*>* stringLiteralList;

    std::string mValue;
    size_t mIndex;

public:
    static StringLiteral* getStringLiteral(size_t);

    StringLiteral(std::string);
    std::string getValue();
    size_t getIndex();
    std::string getAsString();
};

#endif //PILANG_STRINGLITERAL_H
