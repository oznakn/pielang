#include "value.h"

#include <string>

#include "definitions.h"
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
    return new Value((float) std::stoul(s));
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

Value::Value() {}

Value::Value(bool b) {
    this->mValueType = Value::VALUE_TYPE_BOOL;
    this->mBoolValue = b;
}

Value::Value(int i) {
    this->mValueType = Value::VALUE_TYPE_INT;
    this->mFloatValue = (float) i;
}

Value::Value(float f) {
    this->mValueType = Value::VALUE_TYPE_FLOAT;
    this->mFloatValue = f;

    if ((int)(f * 10) % 10 == 0) {
        this->mValueType = Value::VALUE_TYPE_INT;
    }
}

Value::Value(double d) {
    this->mValueType = Value::VALUE_TYPE_FLOAT;
    this->mFloatValue = (float) d;

    if ((int)(d * 10) % 10 == 0) {
        this->mValueType = Value::VALUE_TYPE_INT;
    }
}

Value::Value(std::string s) {
    this->mValueType = Value::VALUE_TYPE_STRING;
    this->mStringValue = s;
}

Value::~Value() {
    if (this->mValueList != nullptr) {
        for (auto value : *this->mValueList) {
            value->unlinkWithValue(this);
        }
    }

    delete this->mValueList;
    delete this->mLinkedVariableList;
    delete this->mLinkedValueList;
}

void Value::linkWithVariable(Variable* variable) {
    if (this->mLinkedVariableList == nullptr) {
        this->mLinkedVariableList = new VariableList;
    }

    this->mLinkedVariableList->push_back(variable);
}

void Value::unlinkWithVariable(Variable* variable) {
    if (this->mLinkedVariableList != nullptr) {
        for (size_t i = 0; i < this->mLinkedVariableList->size(); i++) {
            if (this->mLinkedVariableList->at(0) == variable) {
                this->mLinkedVariableList->erase(this->mLinkedVariableList->begin() + i);

                break;
            }
        }

        this->deleteIfNotLinked();
    }
}

void Value::linkWithValue(Value* mLinkedValueList) {
    if (this->mLinkedValueList == nullptr) {
        this->mLinkedValueList = new ValueList;
    }

    this->mLinkedValueList->push_back(mLinkedValueList);
}

void Value::unlinkWithValue(Value* value) {
    if (this->mLinkedValueList != nullptr) {
        for (size_t i = 0; i < this->mLinkedValueList->size(); i++) {
            if (this->mLinkedValueList->at(0) == value) {
                this->mLinkedValueList->erase(this->mLinkedValueList->begin() + i);

                break;
            }
        }

        this->deleteIfNotLinked();
    }
}

std::string Value::getAsString(bool printStringCharsToo) {
    switch (this->mValueType) {
        case Value::VALUE_TYPE_BOOL: {
            return this->mBoolValue ? "true" : "false";
        }

        case Value::VALUE_TYPE_INT: {
            return std::to_string((int) this->mFloatValue);
        }

        case Value::VALUE_TYPE_FLOAT: {
            std::string str = std::to_string(this->mFloatValue);
            str.erase(str.find_last_not_of('0') + 1, std::string::npos); // TODO
            return str;
        }

        case Value::VALUE_TYPE_STRING: {
            if (!printStringCharsToo) return this->mStringValue;
            return Options::STRING_CHAR + this->mStringValue + Options::STRING_CHAR;
        }

        case Value::VALUE_TYPE_TUPLE: {
            std::string result = "";
            if (this->mValueList != nullptr) {
                for (auto value : *this->mValueList) {
                    result += value->getAsString(false) + ",";
                }
            }
            return result;
        }

        default: {
            return "undefined";
        }
    }
}

ValueType Value::getValueType() {
    return this->mValueType;
}

bool Value::getBoolValue() {
    return this->mBoolValue;
}

int Value::getIntValue() {
    return (int) this->mFloatValue;
}

float Value::getFloatValue() {
    return this->mFloatValue;
}

std::string Value::getStringValue() {
    return this->mStringValue;
}

void Value::deleteIfNotLinked() {
    if ((this->mLinkedVariableList == nullptr || this->mLinkedVariableList->empty()) &&
        (this->mLinkedValueList == nullptr || this->mLinkedValueList->empty())) {

        delete this;
    }
}

void Value::addValue(Value* value) {
    if (this->mValueType != VALUE_TYPE_TUPLE) {
        this->mValueType = VALUE_TYPE_TUPLE;

        this->mValueList = new ValueList;
    }

    this->mValueList->push_back(value);
    value->linkWithValue(this);
}

ValueList* Value::getValueList() {
    return this->mValueList;
}

ValueList* Value::getAsValueList() {
    if (this->mValueType != VALUE_TYPE_TUPLE) {
        auto valueList = new ValueList;
        valueList->push_back(this);
        return valueList;
    }

    return this->getValueList();
}
