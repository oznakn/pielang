#ifndef PILANG_STRINGLITERAL_H
#define PILANG_STRINGLITERAL_H

#include <string>
#include <vector>

using namespace std;

class StringLiteral {
private:
    static vector<StringLiteral*>* stringLiteralList;

    string mValue;
    size_t mIndex;

public:
    static StringLiteral* getStringLiteral(size_t);

    StringLiteral(string);
    string getValue();
    size_t getIndex();
    string getAsString();
};

#endif //PILANG_STRINGLITERAL_H
