#include "systemfunction.h"

#include <string>
#include <vector>

#include "value.h"

SystemFunction::SystemFunction() {}

Value* SystemFunction::run(std::vector<Value*>* arguments) {
    return new Value();
}