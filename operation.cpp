#include "operation.h"

#include <math.h>
#include <string>

#include "scope.h"
#include "value.h"
#include "stringutils.h"

OperationType Operation::parseOperationType(std::string & s) {
    s = StringUtils::trim(s);

    if (s == "+") return Operation::OPERATION_TYPE_ADDITION;
    if (s == "-") return Operation::OPERATION_TYPE_SUBTRACTION;
    if (s == "*") return Operation::OPERATION_TYPE_MULTIPLICATION;
    if (s == "/") return Operation::OPERATION_TYPE_DIVISION;
    if (s == "^") return Operation::OPERATION_TYPE_POWER;
    if (s == "%") return Operation::OPERATION_TYPE_MOD;
    if (s == ",") return Operation::OPERATION_TYPE_COMBINATION;

    return OPERATION_TYPE_NONE;
}

Operation::Operation(Scope* scope, Value* firstValue, Value* lastValue, OperationType operationType) {
    this->mScope = scope;
    this->mFirstValue = firstValue;
    this->mLastValue = lastValue;
    this->mOperationType = operationType;
}

Operation::Operation(Scope* scope, Value* firstValue, Value* lastValue, std::string operationTypeString) {
    this->mScope = scope;
    this->mFirstValue = firstValue;
    this->mLastValue = lastValue;

    this->mOperationType = Operation::parseOperationType(operationTypeString);
}

Operation::~Operation() {
    this->mFirstValue->deleteIfNotLinked();
    this->mLastValue->deleteIfNotLinked();
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

    return new Value;
}

Value* Operation::runMultiplicationOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() * this->mLastValue->getFloatValue());
    }

    return new Value;
}

Value* Operation::runDivisionOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(this->mFirstValue->getFloatValue() / this->mLastValue->getFloatValue());
    }

    return new Value;
}

Value* Operation::runPowerOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT || this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT || this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT)) {
        return new Value(std::pow(this->mFirstValue->getFloatValue(), this->mLastValue->getFloatValue()));
    }

    return new Value;
}

Value* Operation::runModOperation() {
    if ((this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT) &&
        (this->mLastValue->getValueType() == Value::VALUE_TYPE_INT)) {
        return new Value((float) (this->mFirstValue->getIntValue() % this->mLastValue->getIntValue()));
    }

    return new Value;
}

Value* Operation::runCombinationOperation() {
    auto value = new Value;

    if (this->mLastValue->getValueType() == Value::VALUE_TYPE_TUPLE) {
        value->addValue(this->mFirstValue->getValueList()->at(0));
    }
    else {
        value->addValue(this->mFirstValue);
    }

    if (this->mLastValue->getValueType() == Value::VALUE_TYPE_TUPLE) {
        value->addValue(this->mLastValue->getValueList()->at(0));
    }
    else {
        value->addValue(this->mLastValue);
    }

    return value;
}

Value* Operation::run() {
    switch (this->mOperationType) {
        case Operation::OPERATION_TYPE_ADDITION: return this->runAdditionOperation();
        case Operation::OPERATION_TYPE_SUBTRACTION: return this->runSubtractionOperation();
        case Operation::OPERATION_TYPE_MULTIPLICATION: return this->runMultiplicationOperation();
        case Operation::OPERATION_TYPE_DIVISION: return this->runDivisionOperation();
        case Operation::OPERATION_TYPE_POWER: return this->runPowerOperation();
        case Operation::OPERATION_TYPE_MOD: return this->runModOperation();
        case Operation::OPERATION_TYPE_COMBINATION: return this->runCombinationOperation();
        default: return new Value;
    }
}