#ifndef PILANG_SYSTEMFUNCTION_H
#define PILANG_SYSTEMFUNCTION_H

#include <string>
#include <vector>

#include "function.h"

class Value;

typedef Value* (*CallbackFunction)(std::vector<Value*>*);

class SystemFunction: public Function {
private:
    std::string mFunctionName;
    size_t mParameterCount;
    CallbackFunction mCallbackFunction;

public:
    SystemFunction(std::string, size_t, CallbackFunction);
    std::string getFunctionName();
    size_t getParameterCount();
    Value* run(std::vector<Value*>*);
};

#endif //PILANG_SYSTEMFUNCTION_H
