#ifndef PIELANG_OPERATION_H
#define PIELANG_OPERATION_H

#include <string>
#include <vector>

#include "definitions.h"

class Scope;
class Value;
class Operator;

class Operation {
private:
    Scope* mScope;
    Operator* mOperator;
    Value* mFirstValue = nullptr;
    Value* mSecondValue = nullptr;

public:
    Operation(Scope*, Value*, Operator*);
    Operation(Scope*, Value*, Value*, Operator*);
    ~Operation();

    Value* run();
};


#endif //PIELANG_OPERATION_H
