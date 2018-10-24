#include "operator.h"

#include <string>
#include <vector>
#include <cmath>

#include "definitions.h"
#include "value.h"
#include "scope.h"
#include "variable.h"
#include "logger.h"
#include "stringutils.h"

std::vector<Operator*>* Operator::operatorList = new std::vector<Operator*>;

void Operator::addOperator(Operator* anOperator) {
    Operator::operatorList->push_back(anOperator);
}

Operator* Operator::createOperator(OperatorType operatorType, OperatorAssociativeType operatorAssociativeType, OperatorPrecedence operatorPrecedence, std::string operatorString, OperatorFunctionCallback operatorFunctionCallback) {
    auto anOperator = new Operator(operatorType, operatorAssociativeType, operatorPrecedence, operatorString, operatorFunctionCallback);

    Operator::addOperator(anOperator);

    return anOperator;
}

Operator* Operator::getOperatorFromOperatorString(std::string operatorString) {
    for ( auto anOperator : *Operator::operatorList) {
        if (anOperator->getOperatorString() == operatorString) return anOperator;
    }

    return nullptr;
}

Operator* Operator::getOperatorFromOperatorStringWithOperatorType(std::string operatorString, OperatorType operatorType) {
    for (auto anOperator : *Operator::operatorList) {
        if (anOperator->getOperatorString() == operatorString && anOperator->getOperatorType() == operatorType) return anOperator;
    }

    return nullptr;
}

Operator* Operator::getOperatorFromPartOfOperatorString(std::string s) {
    for ( auto anOperator : *Operator::operatorList) {
        if (StringUtils::contains(anOperator->getOperatorString(), s)) return anOperator;
    }

    return nullptr;
}

Operator* Operator::getOperatorFromPartOfOperatorStringWithOperatorType(std::string s, OperatorType operatorType) {
    for ( auto anOperator : *Operator::operatorList) {
        if (StringUtils::contains(anOperator->getOperatorString(), s) && anOperator->getOperatorType() == operatorType) return anOperator;
    }

    return nullptr;
}

void Operator::init() {
    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_RIGHT,
            9,
            "^",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(std::pow(firstValue->getFloatValue(), secondValue->getFloatValue()));
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            8,
            "*",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() * secondValue->getFloatValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            8,
            "/",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() / secondValue->getFloatValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            8,
            "%", 
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT)) {
                    return new Value((float) (firstValue->getIntValue() % secondValue->getIntValue()));
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            7,
            "+", 
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() + secondValue->getFloatValue());
                }

                return new Value(firstValue->getAsString() + secondValue->getAsString());
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            7,
            "-",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() - secondValue->getFloatValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            6,
            "<",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() < secondValue->getFloatValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            6,
            "<=",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() <= secondValue->getFloatValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            6,
            ">", 
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() > secondValue->getFloatValue());
                }

                return Value::undefined;
            });


    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            6,
            ">=",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() >= secondValue->getFloatValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            5,
            "==",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() == secondValue->getFloatValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            5,
            "!=",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if ((firstValue->getValueType() == Value::VALUE_TYPE_INT || firstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
                    (secondValue->getValueType() == Value::VALUE_TYPE_INT || secondValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
                    return new Value(firstValue->getFloatValue() != secondValue->getFloatValue());
                }

                return Value::undefined;
            });


    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            1,
            "=",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                std::string variableName = firstValue->getRepresentation();

                if (scope->hasVariable(variableName)) {
                    scope->getVariable(variableName)->changeValue(secondValue->createNotLinkedInstance());
                }
                else {
                    scope->createVariable(variableName, secondValue->createNotLinkedInstance());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_BINARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            2,
            ",",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                auto newValue = new Value;

                if (firstValue->getValueType() == Value::VALUE_TYPE_COMBINED) {
                    for (auto value : *firstValue->getValueList()) {
                        newValue->addValue(value);
                    }
                }
                else {
                    newValue->addValue(firstValue);
                }

                if (secondValue->getValueType() == Value::VALUE_TYPE_COMBINED) {
                    for (auto value : *secondValue->getValueList()) {
                        newValue->addValue(value);
                    }
                }
                else {
                    newValue->addValue(secondValue);
                }

                return newValue;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_UNARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            0,
            "!",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if (firstValue->getValueType() == Value::VALUE_TYPE_BOOL) {
                    return new Value(!firstValue->getBoolValue());
                }

                return Value::undefined;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_UNARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            0,
            "+",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                return firstValue;
            });

    Operator::createOperator(
            Operator::OPERATOR_TYPE_UNARY,
            Operator::OPERATOR_ASSOCIATIVE_TYPE_LEFT,
            0,
            "-",
            [](Scope* scope, Value* firstValue, Value* secondValue) {
                if (firstValue->getValueType() == Value::VALUE_TYPE_FLOAT || firstValue->getValueType() == Value::VALUE_TYPE_INT) {
                    return new Value(-1 * firstValue->getFloatValue());
                }

                return Value::undefined;
            });
}

Operator::Operator(OperatorType operatorType, OperatorAssociativeType operatorAssociativeType, OperatorPrecedence operatorPrecedence, std::string operatorString, OperatorFunctionCallback operatorFunctionCallback) {
    this->mOperatorType = operatorType;
    this->mOperatorAssociativeType = operatorAssociativeType;
    this->mOperatorPrecedence = operatorPrecedence;
    this->mOperatorString = operatorString;
    this->mOperatorFunctionCallback = operatorFunctionCallback;
}

Value* Operator::run(Scope* scope, Value* firstValue, Value* secondValue) {
    return this->mOperatorFunctionCallback(scope, firstValue, secondValue);
}

std::string Operator::getOperatorString() {
    return this->mOperatorString;
}

OperatorType Operator::getOperatorType() {
    return this->mOperatorType;
}

OperatorAssociativeType Operator::getOperatorAssociativeType() {
    return this->mOperatorAssociativeType;
}

OperatorPrecedence Operator::getOperatorPrecedence() {
    return this->mOperatorPrecedence;
}