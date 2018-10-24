#include "system.h"

#define PI 3.141592653589793238462643383279502884f

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

Scope* System::mainScope = nullptr;

float radianToDegree(float f) {
    return f * PI / 180;
}

void System::init(Scope* mainScope) {
    System::mainScope = mainScope;

    mainScope->createVariable("undefined", Value::undefined);

    mainScope->createSystemFunction("get_undefined",  [](ValueList* arguments) {
        return Value::undefined;
    });

    mainScope->createSystemFunction("import",  [](ValueList* arguments) {
        Logger::println("Imported: " + arguments->at(0)->getAsString());
        return Value::undefined;
    });

    mainScope->createSystemFunction("print",  [](ValueList* arguments) {
        std::string result;
        size_t size = arguments->size();

        for (size_t i = 0; i < size; i++) {
            result += arguments->at(i)->getAsString();

            if (i < size - 1) result += " ";
        }

        Logger::print(result);
        return Value::undefined;
    });

    mainScope->createSystemFunction("println", [](ValueList* arguments) {
        std::string result;
        size_t size = arguments->size();

        for (size_t i = 0; i < size; i++) {
            result += arguments->at(i)->getAsString();

            if (i < size - 1) result += " ";
        }

        Logger::println(result);
        return Value::undefined;
    });

    mainScope->createSystemFunction("get_time", [](ValueList* arguments) {
        return new Value(((float)(clock()*1000)/CLOCKS_PER_SEC));
    });

    mainScope->createSystemFunction("n_number", [](ValueList* arguments) {
        float value = arguments->at(0)->getFloatValue();
        int sfCount = arguments->at(1)->getIntValue();

        int factor = (float) pow(10, sfCount);
        value = ((float) round(value*factor)) / factor;

        return new Value(value);
    });


    Object* mathObject = mainScope->createObject("math");
    mathObject->createVariable("PI", new Value(PI));

    mathObject->createSystemFunction("radian_to_degree", [](ValueList* arguments) {
        return new Value(radianToDegree(arguments->at(0)->getFloatValue()));
    });

    mathObject->createSystemFunction("sqrt", [](ValueList* arguments) {
        return new Value(sqrt(arguments->at(0)->getFloatValue()));
    });

    mathObject->createSystemFunction("sin", [](ValueList* arguments) {
        return new Value(sin(radianToDegree(arguments->at(0)->getFloatValue())));
    });

    mathObject->createSystemFunction("cos", [](ValueList* arguments) {
        return new Value(cos(radianToDegree(arguments->at(0)->getFloatValue())));
    });

    mathObject->createSystemFunction("tan", [](ValueList* arguments) {
        return new Value(tan(radianToDegree(arguments->at(0)->getFloatValue())));
    });

    mathObject->createSystemFunction("cot", [](ValueList* arguments) {
        return new Value(1 / tan(radianToDegree(arguments->at(0)->getFloatValue())));
    });
}

void System::initForScope(Scope* scope) {
    scope->createSystemFunction("return", [scope] (ValueList* arguments) -> Value* {
        scope->setScopeResult(arguments->at(0));
        return Value::undefined;
    });
}