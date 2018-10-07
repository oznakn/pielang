#include "function.h"

#include <string>
#include <vector>

#include "value.h"

std::string Function::getFunctionName() {
    return "";
}

size_t Function::getParameterCount() {
    return 0;
}

Value* Function::run(std::vector<Value *> *) {
    return new Value();
}