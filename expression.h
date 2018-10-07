#ifndef PILANG_EXPRESSION_H
#define PILANG_EXPRESSION_H

#include <string>
#include <vector>

class Scope;
class Value;

typedef size_t TokenType;

class Expression {
private:
    const static TokenType TOKEN_TYPE_NONE = 0;
    const static TokenType TOKEN_TYPE_OPERATION = 1;
    const static TokenType TOKEN_TYPE_VARIABLE = 2;
    const static TokenType TOKEN_TYPE_VALUE = 2;
    const static TokenType TOKEN_TYPE_START_PARENTHESIS = 4;
    const static TokenType TOKEN_TYPE_END_PARENTHESIS = 5;

    static int getOperatorAssociative(char&);
    static int getOperatorAssociative(std::string&);
    static int getOperatorPrecedence(char&);
    static int getOperatorPrecedence(std::string&);
    static bool isOperator(char&);
    static bool isOperator(std::string&);

    Scope* mScope;
    std::string mContent;
    std::vector<std::string>* mOutputStack;
    std::vector<std::string>* mOperatorStack;

    void runOnToken(std::string&);

public:
    Expression(Scope*, std::string);
    ~Expression();

    Value* run();
};

#endif //PILANG_EXPRESSION_H
