#ifndef PIELANG_EXPRESSION_H
#define PIELANG_EXPRESSION_H

#include <string>
#include <vector>

#include "definitions.h"

class Expression {
private:
    static int getOperatorAssociativeType(std::string);
    static int getOperatorPrecedence(std::string);
    static bool isOperator(std::string);
    static bool isPartOfOperator(std::string);
    static bool isUnaryOperator(std::string);
    static bool isPartOfUnaryOperator(std::string);

    Scope* mScope;
    std::string mContent;
    StringList* mOutputStack;
    StringList* mOperatorStack;

    void runOnToken(std::string, bool = false);

public:
    Expression(Scope*, std::string);
    ~Expression();

    Value* run();
};

#endif //PIELANG_EXPRESSION_H
