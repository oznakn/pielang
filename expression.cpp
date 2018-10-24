#include "expression.h"

#include <string>
#include <vector>

#include "definitions.h"
#include "options.h"
#include "utils.h"
#include "stringutils.h"
#include "operation.h"
#include "operator.h"
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

int Expression::getOperatorAssociativeType(std::string s) {
    auto anOperator = Operator::getOperatorFromOperatorString(s);

    if (anOperator != nullptr) {
        return anOperator->getOperatorAssociativeType();
    }

    return Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT;
}

int Expression::getOperatorPrecedence(std::string s) {
    auto anOperator = Operator::getOperatorFromOperatorString(s);

    if (anOperator != nullptr) {
        return anOperator->getOperatorPrecedence();
    }

    return 0; // functions TODO
}

bool Expression::isUnaryOperator(std::string s) {
    return Operator::getOperatorFromOperatorStringWithOperatorType(s, Operator::OPERATOR_TYPE_UNARY) != nullptr;
}

bool Expression::isPartOfUnaryOperator(std::string s) {
    return Operator::getOperatorFromPartOfOperatorStringWithOperatorType(s, Operator::OPERATOR_TYPE_UNARY) != nullptr;
}

bool Expression::isOperator(std::string s) {
    return Operator::getOperatorFromOperatorString(s) != nullptr;
}

bool Expression::isPartOfOperator(std::string s) { // no need if there is one char operator
    return Operator::getOperatorFromPartOfOperatorString(s) != nullptr;
}

// Shunting-yard algorithm
void Expression::runOnToken(std::string token, bool isUnary) {
    if (Expression::isOperator(token)) {
        while(!this->mOperatorStack->empty() &&
              !isUnary && // not an unary operator
              this->mOperatorStack->at(0) != Options::START_PARENTHESIS_STRING &&
              (
                      !Expression::isOperator(this->mOperatorStack->at(0)) ||
                      Expression::getOperatorPrecedence(this->mOperatorStack->at(0)) > Expression::getOperatorPrecedence(token) ||
                      (Expression::getOperatorPrecedence(this->mOperatorStack->at(0)) == Expression::getOperatorPrecedence(token) && Expression::getOperatorAssociativeType(token) > 0)
              )
                ) {

            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }

        if (isUnary) token = '.' + token; // TODO

        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (this->mScope->hasFunction(token)) {
        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (token == Options::START_PARENTHESIS_STRING) {
        this->mOperatorStack->insert(this->mOperatorStack->begin(), token);
    }
    else if (token == Options::END_PARENTHESIS_STRING) {
        while(!this->mOperatorStack->empty() && this->mOperatorStack->at(0) != Options::START_PARENTHESIS_STRING) {
            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }
        this->mOperatorStack->erase(this->mOperatorStack->begin());
    }
    else {
        this->mOutputStack->push_back(token);

        if (!this->mOperatorStack->empty() && this->mOperatorStack->at(0).at(0) == '.') { // means unary operator
            this->mOutputStack->push_back(this->mOperatorStack->at(0));
            this->mOperatorStack->erase(this->mOperatorStack->begin());
        }
    }
}

Value* Expression::run() {
    std::string token;
    std::string lastToken = "\0";

    // Tokenize algorithm
    for (char c : this->mContent) {
        auto s = std::string(1, c);

        if (c == Options::START_PARENTHESIS_CHAR || c == Options::END_PARENTHESIS_CHAR) {
            if (!token.empty()) this->runOnToken(token);
            token = "";
            lastToken = s;
            this->runOnToken(s);
        }
        else if (c != ' ' &&
                    (
                        (isPartOfOperator(token) && isPartOfOperator(token + s)) ||
                        (!isPartOfOperator(token) && !isPartOfOperator(token + s) && !isPartOfOperator(s))
                    )
                ) {
            token += s;
        }
        else {
            if (!token.empty()) {
                this->runOnToken(token, Expression::isUnaryOperator(token) && (lastToken == Options::START_PARENTHESIS_STRING || lastToken == "\0" || Expression::isOperator(lastToken)));
                lastToken = token;
            }
            token = "";

            if (c != ' ') {
                token += s;
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
        if (s.at(0) == '.') { // TODO
            auto anOperator = Operator::getOperatorFromOperatorStringWithOperatorType(StringUtils::substring(s, 1, s.length()), Operator::OPERATOR_TYPE_UNARY); // TODO

            auto operation = new Operation(this->mScope,
                                                 stack->at(stack->size() - 1),
                                                 anOperator);

            stack->erase(stack->begin() + stack->size() - 1);
            stack->push_back(operation->run());

            delete operation;
        }
        else if (isOperator(s)) {
            auto anOperator = Operator::getOperatorFromOperatorStringWithOperatorType(s, Operator::OPERATOR_TYPE_BINARY);

            auto operation = new Operation(this->mScope,
                    stack->at(stack->size() - 2),
                    stack->at(stack->size() - 1),
                    anOperator);

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
        return Value::undefined;
    }

    auto resultValue = stack->at(0);
    delete stack;

    while (resultValue->getValueType() == Value::VALUE_TYPE_COMBINED) {
        if (resultValue->getValueList()->empty()) return Value::undefined;

        resultValue = resultValue->getValueList()->at(0);
    }

    return resultValue;
}