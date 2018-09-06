#ifndef PILANG_EXPRESSION_H
#define PILANG_EXPRESSION_H

#include <string>

class Scope;
class Value;

class Expression {
private:
    Scope* mScope;
    std::string mContent;

public:
    Expression(Scope*, std::string);
    ~Expression();

    Value* run();
};

#endif //PILANG_EXPRESSION_H
