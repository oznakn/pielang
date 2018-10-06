#include "expression.h"

#include <string>
#include <vector>

#include "options.h"
#include "utils.h"
#include "stringutils.h"
#include "operation.h"
#include "scope.h"
#include "value.h"
#include "logger.h"

Expression::Expression(Scope* scope, std::string content) {
    this->mScope = scope;
    this->mContent = content;
}

Expression::~Expression() {

}

int getOperatorAssociative(char& c) {
    switch (c) {
        case '-': return 1;
        case '+': return 1;
        case '%': return 1;
        case '/': return 1;
        case '*': return 1;
        case '^': return -1;
        default: return 1; // for functions
    }
}


int getOperatorAssociative(std::string& s) {
    return getOperatorAssociative(s.at(0));
}

int getOperatorPrecedence(char& c) {
    switch (c) {
        case '-': return 2;
        case '+': return 2;
        case '%': return 2;
        case '/': return 3;
        case '*': return 3;
        case '^': return 4;

        default: return 5; // means operator is a function
    }
}

int getOperatorPrecedence(std::string& s) {
    return getOperatorPrecedence(s.at(0));
}

bool isOperator(char& c) {
    switch (c) {
        case '-': return true;
        case '+': return true;
        case '%': return true;
        case '/': return true;
        case '*': return true;
        case '^': return true;

        default: return false;
    }
}

bool isOperator(std::string& s) {
    return isOperator(s.at(0));
}

bool Expression::isTopOfOperatorStackIsFunction() {
    return false;
}

bool Expression::isTopOfOperatorStackIsGreaterPrecedenceOperator(char& c) {
    return getOperatorPrecedence(this->mOperatorStack->at(0)) > getOperatorPrecedence(c);
}

bool Expression::isTopOfOperatorStackIsLessOrEqualPrecedenceOperator(char& c) {
    return !Expression::isTopOfOperatorStackIsGreaterPrecedenceOperator(c);
}

// Shunting-yard algorithm
void Expression::runOnToken(std::string& token) {
    if (isOperator(token)) {
        while(!this->mOperatorStack->empty() &&
                (
                    getOperatorPrecedence(this->mOperatorStack->at(0)) > getOperatorPrecedence(token) ||
                    (getOperatorPrecedence(this->mOperatorStack->at(0)) == getOperatorPrecedence(token) && getOperatorAssociative(token) > 0)
                )
            ) {

            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }

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
        // ad to operator stack if it is a function
        this->mOutputStack->push_back(token);
    }
}

Value* Expression::run() {
    std::string token = "";

    // Tokenize algorithm
    for (char c : this->mContent) {
        if (c == ' ' || c == ',') {
            if (!token.empty()) {
                this->runOnToken(token);
                token = "";
            }
        }
        else if (isalnum(c) || c == '.' || c == '~')  { // TODO
            token += c;
        }
        else if (isOperator(c)) {
            if (!token.empty()) {
                this->runOnToken(token);
                token = "";
            }
            token = c;
            this->runOnToken(token);
            token = "";
        }
        else if (c == '(' || c == ')') {
            if (!token.empty()) {
                this->runOnToken(token);
            }

            token = c;
            this->runOnToken(token);
            token = "";
        }
    }

    if (!token.empty()) {
        this->runOnToken(token);
    }

    while(!this->mOperatorStack->empty()) {
        this->mOutputStack->push_back(this->mOperatorStack->at(0));
        this->mOperatorStack->erase(this->mOperatorStack->begin());
    }


    // Reverse Polish Notation
    auto stack = new std::vector<std::string>;
    for(std::string s : *this->mOutputStack) {
        if (isOperator(s)) {
            std::string value2 = stack->at(stack->size() - 1);
            stack->erase(stack->begin() + stack->size() - 1);

            std::string value1 = stack->at(stack->size() - 1);
            stack->erase(stack->begin() + stack->size() - 1);

            Operation* operation = new Operation(this->mScope, this->mScope->parseValue(value1),  this->mScope->parseValue(value2), s);
            stack->push_back(operation->run()->getAsString(true));
            delete operation;
        }
        else {
            stack->push_back(s);
        }
    }

    if (stack->empty()) {
        return new Value();
    }

    auto resultValue = this->mScope->parseValue(stack->at(0));
    delete stack;

    return resultValue;
}