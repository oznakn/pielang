#include "system.h"

#include <string>
#include <vector>
#include <unordered_map>

#include "value.h"
#include "logger.h"
#include "systemfunction.h"

std::unordered_map<std::string, SystemFunction*>* System::systemFunctionMap = new std::unordered_map<std::string, SystemFunction*>;

void System::init() {
    class PrintSystemFunction : public SystemFunction {
        Value* run(std::vector<Value*>* arguments) override {
            Logger::println(arguments->at(0)->getAsString());
            return new Value();
        }
    };

    System::systemFunctionMap->insert(std::make_pair(
            "print", new PrintSystemFunction()
            ));
}

bool System::hasSystemFunction(std::string s) {
    return System::systemFunctionMap->find(s) != System::systemFunctionMap->end();
}

SystemFunction* System::getSystemFunction(std::string s) {
    return System::systemFunctionMap->at(s);
}