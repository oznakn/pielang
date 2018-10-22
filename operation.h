#ifndef PIELANG_OPERATION_H
#define PIELANG_OPERATION_H

#include <string>
#include <vector>

class Scope;
class Value;

typedef size_t OperationType;

class Operation {
private:
    Scope* mScope;
    Value* mFirstValue = nullptr;
    Value* mLastValue = nullptr;
    OperationType mOperationType;

    Value* runPowerOperation();
    Value* runMultiplicationOperation();
    Value* runDivisionOperation();
    Value* runModOperation();
    Value* runAdditionOperation();
    Value* runSubtractionOperation();
    Value* runSmallerOperation();
    Value* runSmallerEqualOperation();
    Value* runBiggerOperation();
    Value* runBiggerEqualOperation();
    Value* runEqualOperation();
    Value* runNotEqualOperation();
    Value* runCombinationOperation();
    Value* runUnaryNotOperation();
    Value* runUnaryPlusOperation();
    Value* runUnaryMinusOperation();

public:
    const static OperationType OPERATION_TYPE_NONE = 0;
    const static OperationType OPERATION_TYPE_ADDITION = 1;
    const static OperationType OPERATION_TYPE_SUBTRACTION = 2;
    const static OperationType OPERATION_TYPE_MULTIPLICATION = 3;
    const static OperationType OPERATION_TYPE_DIVISION = 4;
    const static OperationType OPERATION_TYPE_POWER = 5;
    const static OperationType OPERATION_TYPE_MOD = 6;
    const static OperationType OPERATION_TYPE_SMALLER = 7;
    const static OperationType OPERATION_TYPE_SMALLER_EQUAL = 8;
    const static OperationType OPERATION_TYPE_BIGGER = 9;
    const static OperationType OPERATION_TYPE_BIGGER_EQUAL = 10;
    const static OperationType OPERATION_TYPE_EQUAL = 11;
    const static OperationType OPERATION_TYPE_NOT_EQUAL = 12;
    const static OperationType OPERATION_TYPE_COMBINATION = 13;
    const static OperationType OPERATION_TYPE_UNARY_NOT = 14;
    const static OperationType OPERATION_TYPE_UNARY_PLUS = 15;
    const static OperationType OPERATION_TYPE_UNARY_MINUS = 16;

    static OperationType parseOperationType(std::string&);

    Operation(Scope*, Value*, std::string);
    Operation(Scope*, Value*, Value*, std::string);
    ~Operation();

    Value* run();
};


#endif //PIELANG_OPERATION_H
