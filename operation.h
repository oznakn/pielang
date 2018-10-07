#ifndef PILANG_OPERATION_H
#define PILANG_OPERATION_H

#include <string>
#include <vector>

class Scope;
class Value;

typedef size_t OperationType;

class Operation {
private:
    Scope* mScope;
    Value* mFirstValue;
    Value* mLastValue;
    OperationType mOperationType;

    Value* runAdditionOperation();
    Value* runSubtractionOperation();
    Value* runMultiplicationOperation();
    Value* runDivisionOperation();
    Value* runPowerOperation();
    Value* runModOperation();
    Value* runCombinationOperation();

public:
    const static OperationType OPERATION_TYPE_NONE = 0;
    const static OperationType OPERATION_TYPE_ADDITION = 1;
    const static OperationType OPERATION_TYPE_SUBTRACTION = 2;
    const static OperationType OPERATION_TYPE_MULTIPLICATION = 3;
    const static OperationType OPERATION_TYPE_DIVISION = 4;
    const static OperationType OPERATION_TYPE_POWER = 5;
    const static OperationType OPERATION_TYPE_MOD = 6;
    const static OperationType OPERATION_TYPE_COMBINATION = 7;

    static OperationType parseOperationType(std::string&);

    Operation(Scope*, Value*, Value*, OperationType);
    Operation(Scope*, Value*, Value*, std::string);
    ~Operation();

    Value* run();
};


#endif //PILANG_OPERATION_H
