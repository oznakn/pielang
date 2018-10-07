#include "system.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <ctime>

#include "definitions.h"
#include "value.h"
#include "variable.h"
#include "object.h"
#include "scope.h"
#include "logger.h"
#include "systemfunction.h"

Value* importCallback(ValueList* arguments) {
    Logger::println("Imported: " + arguments->at(0)->getAsString());
    return new Value();
}

Value* printCallback(ValueList* arguments) {
    Logger::print(arguments->at(0)->getAsString());
    return new Value();
}

Value* printlnCallback(ValueList* arguments) {
    Logger::println(arguments->at(0)->getAsString());
    return new Value();
}

Value* getTimeCallback(ValueList* arguments) {
    return new Value(((float)(clock()*1000)/CLOCKS_PER_SEC));
}

Value* sinCallback(ValueList* arguments) {
    return new Value((float)(sin((double) arguments->at(0)->getFloatValue())));
}

Value* cosCallback(ValueList* arguments) {
    return new Value((float)(cos((double) arguments->at(0)->getFloatValue())));

}

Value* tanCallback(ValueList* arguments) {
    return new Value((float)(tan((double) arguments->at(0)->getFloatValue())));
}

Value* cotCallback(ValueList* arguments) {
    return new Value((float)(1/ tan((double) arguments->at(0)->getFloatValue())));
}

Value* nNumberCallback(ValueList* arguments) {
    float value = arguments->at(0)->getFloatValue();
    int sfCount = arguments->at(1)->getIntValue();

    int factor = (float) pow(10, sfCount);
    value = ((float) round(value*factor)) / factor;

    return new Value(value);
}

Scope* System::mainScope = nullptr;

void System::init(Scope* mainScope) {
    System::mainScope = mainScope;

    mainScope->createSystemFunction("import",  importCallback);
    mainScope->createSystemFunction("print",  printCallback);
    mainScope->createSystemFunction("println",  printlnCallback);
    mainScope->createSystemFunction("get_time", getTimeCallback);
    mainScope->createSystemFunction("n_number", nNumberCallback);

    Object* mathObject = mainScope->createObject("math");
    mathObject->createVariable("PI", new Value(3.14f));
    mathObject->createSystemFunction("sin", sinCallback);
    mathObject->createSystemFunction("cos", cosCallback);
    mathObject->createSystemFunction("tan", tanCallback);
    mathObject->createSystemFunction("cot", cotCallback);
}