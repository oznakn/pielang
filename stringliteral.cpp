#include "stringliteral.h"

#include <string>
#include <vector>
#include "options.h"

vector<StringLiteral*>* StringLiteral::stringLiteralList = new vector<StringLiteral*>;

StringLiteral* StringLiteral::getStringLiteral(size_t index) {
    if (index < StringLiteral::stringLiteralList->size()) {
        return StringLiteral::stringLiteralList->at(index);
    }

    return nullptr;
}

StringLiteral::StringLiteral(string value) {
    this->mValue = value;

    StringLiteral::stringLiteralList->push_back(this);

    this->mIndex = StringLiteral::stringLiteralList->size() - 1;
}

string StringLiteral::getValue() {
    return this->mValue;
}

size_t StringLiteral::getIndex() {
    return this->mIndex;
}

string StringLiteral::getAsString() {
    return Options::STRING_LITERAL_CHAR + to_string(this->getIndex()) + Options::STRING_LITERAL_CHAR;
}
