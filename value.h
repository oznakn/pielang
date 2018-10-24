#ifndef PIELANG_VALUE_H
#define PIELANG_VALUE_H

#include <string>
#include <vector>

#include "definitions.h"

typedef size_t ValueType;

class Value {
private:
    bool mBoolValue;
    float mFloatValue;
    std::string mStringValue;
    std::string mRepresentation;

    ValueType mValueType = VALUE_TYPE_UNDEFINED;
    ValueList* mValueList = nullptr;

    Variable* mLinkedVariable = nullptr;
    ValueList* mLinkedValueList = nullptr;

public:
    const static ValueType VALUE_TYPE_UNDEFINED = 0;
    const static ValueType VALUE_TYPE_BOOL = 1;
    const static ValueType VALUE_TYPE_INT = 2;
    const static ValueType VALUE_TYPE_FLOAT = 3;
    const static ValueType VALUE_TYPE_STRING = 4;
    const static ValueType VALUE_TYPE_COMBINED = 5;

    static bool isParseableBool(std::string);
    static bool isParseableInt(std::string);
    static bool isParseableFloat(std::string);
    static bool isParseable(std::string);

    static Value* parseStringToBool(std::string);
    static Value* parseStringToInt(std::string);
    static Value* parseStringToFloat(std::string);
    static Value* parseStringToValue(std::string);

    static Value* undefined;

    Value();
    Value(bool);
    Value(int);
    Value(float);
    Value(double);
    Value(std::string);

    ~Value();

    void addValue(Value*);
    ValueList* getValueList();
    ValueList* getAsValueList();

    void setRepresentation(std::string);
    std::string getRepresentation();

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

    Value* createNotLinkedInstance();
    Variable* getLinkedVariable();
};


#endif //PIELANG_VALUE_H
