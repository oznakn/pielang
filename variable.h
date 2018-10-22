#ifndef PIELANG_VARIABLE_H
#define PIELANG_VARIABLE_H

#include <string>

#include "definitions.h"

class Variable {
private:
    std::string mVariableName;
    Value* mValue;

    ObjectList* mLinkedObjectList = nullptr;

public:
    static bool isValidVariableName(std::string);

    Variable(std::string, Value*);
    ~Variable();

    Value* getValue();
    void changeValue(Value*);

    void linkWithObject(Object*);
    void unlinkWithObject(Object*);
    void deleteIfNotLinked();
};

#endif //PIELANG_VARIABLE_H
