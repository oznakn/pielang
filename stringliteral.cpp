#include "stringliteral.h"

#include <string>
#include <vector>

#include "options.h"

std::vector<StringLiteral*>* StringLiteral::stringLiteralList = new std::vector<StringLiteral*>;

StringLiteral* StringLiteral::getStringLiteral(size_t index) {
    if (index < StringLiteral::stringLiteralList->size()) {
        return StringLiteral::stringLiteralList->at(index);
    }

    return nullptr;
}

StringLiteral::StringLiteral(std::string value) {
    this->mValue = value;

    StringLiteral::stringLiteralList->push_back(this);

    this->mIndex = StringLiteral::stringLiteralList->size() - 1;
}

std::string StringLiteral::getValue() {
    return this->mValue;
}

size_t StringLiteral::getIndex() {
    return this->mIndex;
}

std::string StringLiteral::getAsString() {
    return Options::STRING_LITERAL_CHAR + std::to_string(this->getIndex()) + Options::STRING_LITERAL_CHAR;
}
