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
    float mFloatValue;
    std::string mStringValue;

    ValueType mValueType = VALUE_TYPE_UNDEFINED;
    std::vector<Variable*>* mLinkedVariableList = nullptr;
    std::vector<Value*>* mLinkedValueList = nullptr;
    std::vector<Value*>* mValueList = nullptr;

public:
    const static ValueType VALUE_TYPE_UNDEFINED = 0;
    const static ValueType VALUE_TYPE_BOOL = 1;
    const static ValueType VALUE_TYPE_INT = 2;
    const static ValueType VALUE_TYPE_FLOAT = 3;
    const static ValueType VALUE_TYPE_STRING = 4;
    const static ValueType VALUE_TYPE_TUPLE = 5;

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
    Value(int);
    Value(float);
    Value(std::string);

    ~Value();

    void addValue(Value*);
    std::vector<Value*>* getValueList();
    std::vector<Value*>* getAsValueList();

    void linkWithVariable(Variable*);
    void unlinkWithVariable(Variable*);
    void linkWithValue(Value*);
    void unlinkWithValue(Value*);
    std::string getAsString(bool = false);

    ValueType getValueType();
    bool getBoolValue();
    int getIntValue();
    float getFloatValue();
    std::string getStringValue();

    void deleteIfNotLinked();
};


#endif //PILANG_VALUE_H
