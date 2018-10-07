#include "system.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <ctime>

#include "value.h"
#include "variable.h"
#include "scope.h"
#include "logger.h"
#include "systemfunction.h"

Value* printCallback(std::vector<Value*>* arguments) {
    Logger::print(arguments->at(0)->getAsString());
    return new Value();
}

Value* printlnCallback(std::vector<Value*>* arguments) {
    Logger::println(arguments->at(0)->getAsString());
    return new Value();
}

Value* getTimeCallback(std::vector<Value*>* arguments) {
    return new Value((float)((clock()*1000)/CLOCKS_PER_SEC));
}

Value* sinCallback(std::vector<Value*>* arguments) {
    return new Value((float)(sin(arguments->at(0)->getFloatValue())));
}

Value* cosCallback(std::vector<Value*>* arguments) {
    return new Value((float)(cos(arguments->at(0)->getFloatValue())));

}

Value* tanCallback(std::vector<Value*>* arguments) {
    return new Value((float)(tan(arguments->at(0)->getFloatValue())));
}

Value* cotCallback(std::vector<Value*>* arguments) {
    return new Value((float)(1/ tan(arguments->at(0)->getFloatValue())));
}

Value* nNumberCallback(std::vector<Value*>* arguments) {
    float value = arguments->at(0)->getFloatValue();
    int sfCount = arguments->at(1)->getIntValue();

    int factor = (float) pow(10, sfCount);
    value = ((float) round(value*factor)) / factor;

    return new Value(value);
}

void System::init(Scope* mainScope) {
    mainScope->createSystemFunction("print", 1, printCallback);
    mainScope->createSystemFunction("println", 1, printlnCallback);
    mainScope->createSystemFunction("get_time", 0, getTimeCallback);
    mainScope->createSystemFunction("sin", 1, sinCallback);
    mainScope->createSystemFunction("cos", 1, cosCallback);
    mainScope->createSystemFunction("tan", 1, tanCallback);
    mainScope->createSystemFunction("cot", 1, cotCallback);
    mainScope->createSystemFunction("n_number", 2, nNumberCallback);

    mainScope->addVariable("PI", new Variable("PI", new Value((float)3.14)));
}