#include "stringliteral.h"

#include <string>
#include <vector>

#include "options.h"
#include "stringutils.h"
#include "value.h"

bool StringLiteral::isValidStringLiteral(std::string s) {
    s = StringUtils::trim(s);

    return (s.at(0) == Options::STRING_LITERAL_CHAR && s.at(s.length() - 1) == Options::STRING_LITERAL_CHAR);
}

std::vector<StringLiteral*>* StringLiteral::stringLiteralList = new std::vector<StringLiteral*>;

StringLiteral* StringLiteral::getStringLiteral(size_t index) {
    if (index < StringLiteral::stringLiteralList->size()) {
        return StringLiteral::stringLiteralList->at(index);
    }

    return nullptr;
}

StringLiteral* StringLiteral::findStringLiteral(std::string s) {
    size_t index = std::stoul(StringUtils::substring(s, 1, s.length() - 1));

    return StringLiteral::getStringLiteral(index);
}

StringLiteral::StringLiteral(std::string s) {
    this->mString = s;
    this->mIndex = StringLiteral::stringLiteralList->size();

    StringLiteral::stringLiteralList->push_back(this);
}

Value* StringLiteral::getValue() {
    return new Value(this->mString);
}

size_t StringLiteral::getIndex() {
    return this->mIndex;
}

std::string StringLiteral::getAsString() {
    return Options::STRING_LITERAL_CHAR + std::to_string(this->getIndex()) + Options::STRING_LITERAL_CHAR;
}
