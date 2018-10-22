#ifndef PIELANG_SYSTEMFUNCTION_H
#define PIELANG_SYSTEMFUNCTION_H

#include <string>
#include <vector>

#include "definitions.h"
#include "function.h"


class SystemFunction: public Function {
private:
    std::string mFunctionName;
    FunctionCallback mFunctionCallback;

public:
    SystemFunction(std::string, FunctionCallback);
    std::string getFunctionName() override;
    virtual Value* run(ValueList*) override;
};

#endif //PIELANG_SYSTEMFUNCTION_H
