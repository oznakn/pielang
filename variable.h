#ifndef PILANG_VARIABLE_H
#define PILANG_VARIABLE_H

#include <string>

class Value;

class Variable {
private:
    std::string mVariableName;
    Value* mValue;

public:
    static bool isValidVariableName(std::string);

    Variable(std::string, Value*);
    ~Variable();

    Value* getValue();
    void changeValue(Value*);
};

#endif //PILANG_VARIABLE_H
