#include "value.h"

#include <string>

#include "options.h"
#include "stringutils.h"

bool Value::isParseableBool(std::string s) {
    s = StringUtils::trim(s);
    return s == Options::BOOL_TRUE_STRING || s == Options::BOOL_FALSE_STRING;
}

bool Value::isParseableInt(std::string s) {
    s = StringUtils::trim(s);
    for (size_t i = 0; i < s.length() ;i++) {
        if (!std::isdigit(s.at(i)) && s.at(i) != '-') {
            return false;
        }
    }

    return true;
}

bool Value::isParseableFloat(std::string s) {
    s = StringUtils::trim(s);
    for (size_t i = 0; i < s.length() ;i++) {
        if (!std::isdigit(s.at(i)) && s.at(i) != '-' && s.at(i) != '.') {
            return false;
        }
    }

    return true;
}

bool Value::isParseable(std::string s) {
    return Value::isParseableBool(s) || Value::isParseableInt(s) || Value::isParseableFloat(s) ;
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

Value* Value::parseStringToValue(std::string s) {
    if (Value::isParseableBool(s)) {
        return Value::parseStringToBool(s);
    }
    else if (Value::isParseableInt(s)) {
        return Value::parseStringToInt(s);
    }
    else if (Value::isParseableFloat(s)) {
        return Value::parseStringToFloat(s);
    }

    return nullptr;
}

Value::Value() {
    this->mLinkedVariableList = new std::vector<Variable*>;
}

Value::Value(bool b) {
    this->mValueType = Value::VALUE_TYPE_BOOL;
    this->mBoolValue = b;
    this->mLinkedVariableList = new std::vector<Variable*>;
}

Value::Value(size_t i) {
    this->mValueType = Value::VALUE_TYPE_INT;
    this->mIntValue = i;

    this->mLinkedVariableList = new std::vector<Variable*>;
}

Value::Value(float f) {
    this->mValueType = Value::VALUE_TYPE_FLOAT;
    this->mFloatValue = f;
    this->mLinkedVariableList = new std::vector<Variable*>;
}

Value::Value(std::string s) {
    this->mValueType = Value::VALUE_TYPE_STRING;
    this->mStringValue = s;
    this->mLinkedVariableList = new std::vector<Variable*>;
}

Value::~Value() {
    delete this->mLinkedVariableList;
}

void Value::linkWithVariable(Variable* variable) {
    if (this->mLinkedVariableList == NULL) {
        this->mLinkedVariableList = new std::vector<Variable*>;
    }
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

std::string Value::getAsString(bool printStringCharsToo) {
    switch (this->mValueType) {
        case Value::VALUE_TYPE_BOOL: return this->mBoolValue ? "true" : "false";
        case Value::VALUE_TYPE_INT: return std::to_string(this->mIntValue);
        case Value::VALUE_TYPE_FLOAT: return std::to_string(this->mFloatValue);
        case Value::VALUE_TYPE_STRING:
            if (!printStringCharsToo) return this->mStringValue;
            return Options::STRING_CHAR + this->mStringValue + Options::STRING_CHAR;
        default: return "undefined";
    }
}

ValueType Value::getValueType() {
    return this->mValueType;
}

bool Value::getBoolValue() {
    return this->mBoolValue;
}

size_t Value::getIntValue() {
    return this->mIntValue;
}

float Value::getFloatValue() {
    return this->mFloatValue;
}

std::string Value::getStringValue() {
    return this->mStringValue;
}

