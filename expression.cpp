#include "expression.h"

#include <string>
#include <vector>

#include "definitions.h"
#include "options.h"
#include "utils.h"
#include "stringutils.h"
#include "operation.h"
#include "scope.h"
#include "value.h"
#include "userfunction.h"
#include "systemfunction.h"
#include "logger.h"
#include "system.h"

/**
 * https://philcurnow.wordpress.com/2015/01/24/conversion-of-expressions-from-infix-to-postfix-notation-in-c-part-2-unary-operators/
 * https://www.wikizero.pro/index.php?q=aHR0cHM6Ly9lbi53aWtpcGVkaWEub3JnL3dpa2kvU2h1bnRpbmcteWFyZF9hbGdvcml0aG0
 * https://www.wikizero.pro/index.php?q=aHR0cHM6Ly9lbi53aWtpcGVkaWEub3JnL3dpa2kvUmV2ZXJzZV9Qb2xpc2hfbm90YXRpb24
 * https://codeburst.io/conversion-of-infix-expression-to-postfix-expression-using-stack-data-structure-3faf9c212ab8
 * */

Expression::Expression(Scope* scope, std::string content) {
    this->mScope = scope;
    this->mContent = content;

    this->mOutputStack = new StringList;
    this->mOperatorStack = new StringList;
}

Expression::~Expression() {

}

int Expression::getOperatorAssociative(std::string s) {
    if (s == "-") return 1;
    if (s == "+") return 1;
    if (s == "%") return 1;
    if (s == "/") return 1;
    if (s == "*") return 1;
    if (s == "^") return -1;
    if (s == ",") return 1;

    return 1;
}

int Expression::getOperatorPrecedence(std::string s) {
    if (s == "-") return 2;
    if (s == "+") return 2;
    if (s == "%") return 2;
    if (s == "/") return 3;
    if (s == "*") return 3;
    if (s == "^") return 4;
    if (s == ",") return 1;

    return 5;
}

bool Expression::isOperator(std::string s) {
    return s == "-" ||
           s == "+" ||
           s == "%" ||
           s == "/" ||
           s == "*" ||
           s == "^" ||
           s == ",";
}

bool Expression::isPartOfOperator(std::string s) {
    return s == "-" ||
           s == "+" ||
           s == "%" ||
           s == "/" ||
           s == "*" ||
           s == "**" ||
           s == "^" ||
           s == ",";
}

// Shunting-yard algorithm
void Expression::runOnToken(std::string token) {
    if (Expression::isOperator(token)) {
        while(!this->mOperatorStack->empty() &&
                this->mOperatorStack->at(0) != "(" &&
                (
                    !Expression::isOperator(this->mOperatorStack->at(0)) ||
                    Expression::getOperatorPrecedence(this->mOperatorStack->at(0)) > Expression::getOperatorPrecedence(token) ||
                    (Expression::getOperatorPrecedence(this->mOperatorStack->at(0)) == Expression::getOperatorPrecedence(token) && Expression::getOperatorAssociative(token) > 0)
                )
            ) {

            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }

        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (this->mScope->hasFunction(token)) {
        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (token == "(") {
        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (token == ")") {
        while(!this->mOperatorStack->empty() && this->mOperatorStack->at(0) != "(") {
            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }
        this->mOperatorStack->erase(this->mOperatorStack->begin());
    }
    else {
        this->mOutputStack->push_back(token);
    }
}

Value* Expression::run() {
    std::string token;

    // Tokenize algorithm
    for (char c : this->mContent) {
        auto s = std::string(1, c);

        if (c == '(' || c == ')') {
            if (!token.empty()) this->runOnToken(token);
            token = "";

            this->runOnToken(s);
        }
        else if (isPartOfOperator(s) == isPartOfOperator(token)) {
            token += s;
        }
        else {
            if (!token.empty()) this->runOnToken(token);
            token = "";

            if (c != ' '){
                token = s;
            }
        }
    }

    if (!token.empty()) {
        this->runOnToken(token);
        token = "";
    }

    while(!this->mOperatorStack->empty()) {
        this->mOutputStack->push_back(this->mOperatorStack->at(0));
        this->mOperatorStack->erase(this->mOperatorStack->begin());
    }

    delete this->mOperatorStack;

    // Reverse Polish Notation
    auto stack = new ValueList;
    for(std::string s : *this->mOutputStack) {
        if (isOperator(s)) {
            Operation* operation = new Operation(this->mScope,
                    stack->at(stack->size() - 2),
                    stack->at(stack->size() - 1),
                    s);

            stack->erase(stack->begin() + stack->size() - 2, stack->begin() + stack->size());
            stack->push_back(operation->run());

            delete operation;
        }
        else if (this->mScope->hasFunction(s)) {
            auto result = this->mScope->getFunction(s)->run(stack->at(stack->size() - 1)->getAsValueList());

            stack->erase(stack->begin() + stack->size() - 1);
            stack->push_back(result);
        }
        else {
            stack->push_back(this->mScope->parseValue(s));
        }
    }

    delete this->mOutputStack;

    if (stack->empty()) {
        return new Value();
    }

    auto resultValue = stack->at(0);
    delete stack;

    return resultValue;
}