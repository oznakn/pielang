#ifndef PILANG_SYSTEMFUNCTION_H
#define PILANG_SYSTEMFUNCTION_H

#include <string>
#include <vector>

#include "definitions.h"
#include "function.h"


class SystemFunction: public Function {
private:
    std::string mFunctionName;
    FunctionCallback* mFunctionCallback;

public:
    SystemFunction(std::string, FunctionCallback*);
    std::string getFunctionName();
    Value* run(ValueList*);
};

#endif //PILANG_SYSTEMFUNCTION_H
