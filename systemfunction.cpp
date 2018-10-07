#include "systemfunction.h"

#include <string>
#include <vector>

#include "value.h"

SystemFunction::SystemFunction(std::string functionName, FunctionCallback* functionCallback) {
    this->mFunctionName = functionName;
    this->mFunctionCallback = functionCallback;
}

Value* SystemFunction::run(ValueList* arguments) {
    return this->mFunctionCallback(arguments);
}

std::string SystemFunction::getFunctionName() {
    return this->mFunctionName;
}