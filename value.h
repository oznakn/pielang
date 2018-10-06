#ifndef PILANG_VALUE_H
#define PILANG_VALUE_H

#include <string>
#include <vector>

typedef size_t ValueType;

class Variable;
class Scope;

class Value {
private:
    bool mBoolValue;
    size_t mIntValue;
    float mFloatValue;
    std::string mStringValue;

    ValueType mValueType = VALUE_TYPE_UNDEFINED;
    std::vector<Variable*>* mLinkedVariableList;

public:
    const static ValueType VALUE_TYPE_UNDEFINED = 0;
    const static ValueType VALUE_TYPE_BOOL = 1;
    const static ValueType VALUE_TYPE_INT = 2;
    const static ValueType VALUE_TYPE_FLOAT = 3;
    const static ValueType VALUE_TYPE_STRING = 4;

    static bool isParseableBool(std::string);
    static bool isParseableInt(std::string);
    static bool isParseableFloat(std::string);
    static bool isParseable(std::string);

    static Value* parseStringToBool(std::string);
    static Value* parseStringToInt(std::string);
    static Value* parseStringToFloat(std::string);
    static Value* parseStringToValue(std::string);

    Value();
    Value(bool);
    Value(size_t);
    Value(float);
    Value(std::string);

    ~Value();

    void linkWithVariable(Variable*);
    void unlinkWithVariable(Variable*);
    std::string getAsString(bool = false);

    ValueType getValueType();
    bool getBoolValue();
    size_t getIntValue();
    float getFloatValue();
    std::string getStringValue();

    void deleteIfNotLinked();
};


#endif //PILANG_VALUE_H
