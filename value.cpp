#include "value.h"

#include <iostream>
#include <string>

#include "definitions.h"
#include "options.h"
#include "logger.h"
#include "stringutils.h"

Value* Value::undefined = new Value;

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
    delete this->mLinkedValueList;
}

void Value::linkWithVariable(Variable* variable) {
    if (this->mLinkedVariable != nullptr) std::cout << "Hoaydaa1" << std::endl;

    this->mLinkedVariable = variable;
}

void Value::unlinkWithVariable(Variable* variable) {
    this->mLinkedVariable = nullptr;

    this->deleteIfNotLinked();
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

        case Value::VALUE_TYPE_COMBINED: {
            std::string result;
            size_t size = this->mValueList->size();

            for (size_t i = 0; i < size; i++) {
                result += this->mValueList->at(i)->getAsString();

                if (i < size - 1) result += " , "; // TODO
            }

            return "(" + result + ")"; // TODO
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
    if ((this->mLinkedVariable== nullptr) &&
        (this->mLinkedValueList == nullptr || this->mLinkedValueList->empty())) {

        delete this;
    }
}

void Value::setRepresentation(std::string s) {
    this->mRepresentation = s;
}


std::string Value::getRepresentation() {
    return this->mRepresentation;
}

void Value::addValue(Value* value) {
    if (this->mValueList == nullptr) {
        this->mValueType = Value::VALUE_TYPE_COMBINED;

        this->mValueList = new ValueList;
    }

    this->mValueList->push_back(value);
    value->linkWithValue(this);
}

ValueList* Value::getValueList() {
    return this->mValueList;
}

ValueList* Value::getAsValueList() {
    auto valueList = new ValueList;

    if (this->mValueType == VALUE_TYPE_COMBINED) {
        for (auto value : *this->mValueList) {
            valueList->push_back(value->createNotLinkedInstance());
        }
    }
    else {
        valueList->push_back(this->createNotLinkedInstance()); // TODO
    }

    return valueList;
}

Value* Value::createNotLinkedInstance() {
    if (this->mLinkedVariable == nullptr) return this;

    if (this->mValueType == Value::VALUE_TYPE_BOOL) {
        return new Value(this->mBoolValue);
    }
    else if (this->mValueType == Value::VALUE_TYPE_INT || this->mValueType == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFloatValue);
    }
    else if (this->mValueType == Value::VALUE_TYPE_STRING) {
        return new Value(this->mStringValue);
    }
    else if (this->mValueType == Value::VALUE_TYPE_COMBINED) {
        auto newValue = new Value;

        for (auto value : *this->mValueList) {
            newValue->addValue(value);
        }

        return newValue;
    }

    return new Value; // TODO
}

Variable* Value::getLinkedVariable() {
    return this->mLinkedVariable;
}