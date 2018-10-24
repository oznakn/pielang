#ifndef PIELANG_OPERATOR_H
#define PIELANG_OPERATOR_H

#include <string>

#include "definitions.h"

class Operator {
private:
    static std::vector<Operator*>* operatorList;

    OperatorType mOperatorType;
    OperatorAssociativeType mOperatorAssociativeType;
    OperatorPrecedence mOperatorPrecedence;
    std::string mOperatorString;
    OperatorFunctionCallback mOperatorFunctionCallback;

public:
    static const OperatorType OPERATOR_TYPE_BINARY = 1;
    static const OperatorType OPERATOR_TYPE_UNARY = 2;
    static const OperatorAssociativeType OPERATOR_ASSOCIATIVE_TYPE_LEFT = 1;
    static const OperatorAssociativeType OPERATOR_ASSOCIATIVE_TYPE_RIGHT = 2;

    static void addOperator(Operator*);
    static Operator* createOperator(OperatorType, OperatorAssociativeType, OperatorPrecedence, std::string, OperatorFunctionCallback);
    static void init();

    static Operator* getOperatorFromOperatorString(std::string);
    static Operator* getOperatorFromOperatorStringWithOperatorType(std::string, OperatorType);
    static Operator* getOperatorFromPartOfOperatorString(std::string);
    static Operator* getOperatorFromPartOfOperatorStringWithOperatorType(std::string, OperatorType);

    Operator(OperatorType, OperatorAssociativeType, OperatorPrecedence, std::string, OperatorFunctionCallback);
    Value* run(Scope*, Value*, Value*);

    std::string getOperatorString();
    OperatorType getOperatorType();
    OperatorAssociativeType getOperatorAssociativeType();
    OperatorPrecedence getOperatorPrecedence();
};


#endif //PIELANG_OPERATOR_H
