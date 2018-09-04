#include "value.h"

#include <string>

#include "options.h"
#include "stringutils.h"

bool Value::isParsableBool(std::string s) {
    s = StringUtils::trim(s);
    return s == Options::BOOL_TRUE_STRING || s == Options::BOOL_FALSE_STRING;
}

bool Value::isParsableInt(std::string s) {
    s = StringUtils::trim(s);
    for (size_t i = 0; i < s.length() ;i++) {
        if (!std::isdigit(s.at(i))) {
            return false;
        }
    }

    return true;
}

bool Value::isParsableFloat(std::string s) {
    s = StringUtils::trim(s);
    for (size_t i = 0; i < s.length() ;i++) {
        if (!std::isdigit(s.at(i)) && s.at(i) != '.') {
            return false;
        }
    }

    return true;
}

bool Value::isParsableString(std::string s) {
    s = StringUtils::trim(s);
    return s.at(0) == Options::STRING_CHAR && s.at(s.length() - 1) == Options::STRING_CHAR;
}

Value* Value::parseStringToBool(std::string s) {
    if (s == Options::BOOL_TRUE_STRING) {
        return new Value(true);
    }
    else if (s == Options::BOOL_FALSE_STRING) {
        return new Value(false);
    }

    return nullptr;
}

Value* Value::parseStringToInt(std::string s) {
    return new Value(std::stoul(s));
}

Value* Value::parseStringToFloat(std::string s) {
    return new Value(std::stof(s));
}

Value* Value::parseStringToString(std::string s) {
    return new Value(StringUtils::substring(s, 1, s.length() - 1));
}

Value* Value::parseStringToValue(std::string s) {
    if (Value::isParsableBool(s)) {
        return Value::parseStringToBool(s);
    }
    else if (Value::isParsableInt(s)) {
        return Value::parseStringToInt(s);
    }
    else if (Value::isParsableFloat(s)) {
        return Value::parseStringToFloat(s);
    }
    else if (Value::isParsableString(s)) {
        return Value::parseStringToString(s);
    }

    return nullptr;
}

Value::Value() {}

Value::Value(bool b) {
    this->mValueType = Value::VALUE_TYPE_BOOL;
    this->mBoolValue = b;
}

Value::Value(size_t i) {
    this->mValueType = Value::VALUE_TYPE_INT;
    this->mIntValue = i;
}

Value::Value(float f) {
    this->mValueType = Value::VALUE_TYPE_FLOAT;
    this->mFloatValue = f;
}

Value::Value(std::string s) {
    this->mValueType = Value::VALUE_TYPE_STRING;
    this->mStringValue = s;
}

Value::~Value() {
    delete this->mLinkedVariableList;
}

void Value::linkWithVariable(Variable* variable) {
    this->mLinkedVariableList->push_back(variable);
}

void Value::unlinkWithVariable(Variable* variable) {
    for (size_t i = 0; i < this->mLinkedVariableList->size(); i++) {
        if (this->mLinkedVariableList->at(0) == variable) {
            this->mLinkedVariableList->erase(this->mLinkedVariableList->begin() + i);
            break;
        }
    }

    if (this->mLinkedVariableList->empty()) {
        delete this;
    }
}