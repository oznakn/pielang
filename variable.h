#ifndef PILANG_VARIABLE_H
#define PILANG_VARIABLE_H

#include <string>

#include "value.h"

class Variable {
public:
    static bool isValidVariableName(std::string);

    Variable(std::string, Value*);
};

#endif //PILANG_VARIABLE_H
