#include "operation.h"

#include <math.h>
#include <string>

#include "scope.h"
#include "value.h"
#include "variable.h"
#include "stringutils.h"

OperationType Operation::parseOperationType(std::string & s) {
    s = StringUtils::trim(s);

    if (s == "^") return Operation::OPERATION_TYPE_POWER;
    if (s == "**") return Operation::OPERATION_TYPE_POWER;
    if (s == "*") return Operation::OPERATION_TYPE_MULTIPLICATION;
    if (s == "/") return Operation::OPERATION_TYPE_DIVISION;
    if (s == "%") return Operation::OPERATION_TYPE_MOD;
    if (s == "+") return Operation::OPERATION_TYPE_ADDITION;
    if (s == "-") return Operation::OPERATION_TYPE_SUBTRACTION;
    if (s == "<") return Operation::OPERATION_TYPE_SMALLER;
    if (s == "<=") return Operation::OPERATION_TYPE_SMALLER_EQUAL;
    if (s == ">") return Operation::OPERATION_TYPE_BIGGER;
    if (s == ">=") return Operation::OPERATION_TYPE_BIGGER_EQUAL;
    if (s == "==") return Operation::OPERATION_TYPE_EQUAL;
    if (s == "!=") return Operation::OPERATION_TYPE_NOT_EQUAL;
    if (s == ",") return Operation::OPERATION_TYPE_COMBINATION;
    if (s == ".!") return Operation::OPERATION_TYPE_UNARY_NOT;
    if (s == ".+") return Operation::OPERATION_TYPE_UNARY_PLUS;
    if (s == ".-") return Operation::OPERATION_TYPE_UNARY_MINUS;

    return OPERATION_TYPE_NONE;
}

Operation::Operation(Scope* scope, Value* firstValue, Value* lastValue, std::string operationTypeString) {
    this->mScope = scope;
    this->mFirstValue = firstValue;
    this->mLastValue = lastValue;

    this->mOperationType = Operation::parseOperationType(operationTypeString);
}

Operation::Operation(Scope* scope, Value* firstValue, std::string operationTypeString) {
    this->mScope = scope;
    this->mFirstValue = firstValue;

    this->mOperationType = Operation::parseOperationType(operationTypeString);
}

Operation::~Operation() {
    if (this->mFirstValue != nullptr) this->mFirstValue->deleteIfNotLinked();

    if (this->mLastValue != nullptr) this->mLastValue->deleteIfNotLinked();
}


Value* Operation::runPowerOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(std::pow(this->mFirstValue->getFloatValue(), this->mLastValue->getFloatValue()));
    }

    return Value::undefined;
}


Value* Operation::runMultiplicationOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() * this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runDivisionOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() / this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}


Value* Operation::runModOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT)) {
        return new Value((float) (this->mFirstValue->getIntValue() % this->mLastValue->getIntValue()));
    }

    return Value::undefined;
}


Value* Operation::runAdditionOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() + this->mLastValue->getFloatValue());
    }

    return new Value(this->mFirstValue->getAsString() + this->mLastValue->getAsString());
}

Value* Operation::runSubtractionOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() - this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runSmallerOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() < this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runSmallerEqualOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() <= this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runBiggerOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() > this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runBiggerEqualOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() >= this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runEqualOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() == this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runNotEqualOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() != this->mLastValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::runCombinationOperation() {
    auto newValue = new Value;

    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_COMBINED) {
        for (auto value : *this->mFirstValue->getValueList()) {
            newValue->addValue(value);
        }
    }
    else {
        newValue->addValue(this->mFirstValue);
    }

    if (this->mLastValue->getValueType() == Value::VALUE_TYPE_COMBINED) {
        for (auto value : *this->mLastValue->getValueList()) {
            newValue->addValue(value);
        }
    }
    else {
        newValue->addValue(this->mLastValue);
    }

    return newValue;
}

Value* Operation::runUnaryNotOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_BOOL) {
        return new Value(!this->mFirstValue->getBoolValue());
    }

    return Value::undefined;
}

Value* Operation::runUnaryPlusOperation() {
    return this->mFirstValue;
}

Value* Operation::runUnaryMinusOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(-1 * this->mFirstValue->getFloatValue());
    }

    return Value::undefined;
}

Value* Operation::run() {
    switch (this->mOperationType) {
        case Operation::OPERATION_TYPE_POWER: return this->runPowerOperation();
        case Operation::OPERATION_TYPE_MULTIPLICATION: return this->runMultiplicationOperation();
        case Operation::OPERATION_TYPE_DIVISION: return this->runDivisionOperation();
        case Operation::OPERATION_TYPE_MOD: return this->runModOperation();
        case Operation::OPERATION_TYPE_ADDITION: return this->runAdditionOperation();
        case Operation::OPERATION_TYPE_SUBTRACTION: return this->runSubtractionOperation();
        case Operation::OPERATION_TYPE_SMALLER: return this->runSmallerOperation();
        case Operation::OPERATION_TYPE_SMALLER_EQUAL: return this->runSmallerEqualOperation();
        case Operation::OPERATION_TYPE_BIGGER: return this->runBiggerOperation();
        case Operation::OPERATION_TYPE_BIGGER_EQUAL: return this->runBiggerEqualOperation();
        case Operation::OPERATION_TYPE_EQUAL: return this->runEqualOperation();
        case Operation::OPERATION_TYPE_NOT_EQUAL: return this->runNotEqualOperation();
        case Operation::OPERATION_TYPE_COMBINATION: return this->runCombinationOperation();
        case Operation::OPERATION_TYPE_UNARY_NOT: return this->runUnaryNotOperation();
        case Operation::OPERATION_TYPE_UNARY_PLUS: return this->runUnaryPlusOperation();
        case Operation::OPERATION_TYPE_UNARY_MINUS: return this->runUnaryMinusOperation();
        default: return Value::undefined;
    }
}