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

    Scope* mScope;
    std::string mContent;
    std::vector<std::string>* mOutputStack = new std::vector<std::string>; // TODO
    std::vector<std::string>* mOperatorStack = new std::vector<std::string>;

    bool isTopOfOperatorStackIsFunction();
    bool isTopOfOperatorStackIsGreaterPrecedenceOperator(char&);
    bool isTopOfOperatorStackIsLessOrEqualPrecedenceOperator(char&);

public:
    Expression(Scope*, std::string);
    ~Expression();

    void runOnToken(std::string&);
    Value* run();
};

#endif //PILANG_EXPRESSION_H
