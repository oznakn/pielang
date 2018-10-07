#include "systemfunction.h"

#include <string>
#include <vector>

#include "value.h"

SystemFunction::SystemFunction(std::string functionName, size_t parameterCount, CallbackFunction callbackFunction) {
    this->mFunctionName = functionName;
    this->mParameterCount = parameterCount;
    this->mCallbackFunction = callbackFunction;
}

Value* SystemFunction::run(std::vector<Value*>* arguments) {
    return this->mCallbackFunction(arguments);
}

std::string SystemFunction::getFunctionName() {
    return this->mFunctionName;
}

size_t SystemFunction::getParameterCount() {
    return this->mParameterCount;
}