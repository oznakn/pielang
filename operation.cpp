#include "operation.h"

#include <math.h>
#include <string>

#include "scope.h"
#include "value.h"
#include "stringutils.h"

OperationType Operation::parseOperationType(std::string s) {
    s = StringUtils::trim(s);

    if (s == "+") return Operation::OPERATION_TYPE_ADDITION;
    if (s == "-") return Operation::OPERATION_TYPE_SUBSTRACTION;
    if (s == "*") return Operation::OPERATION_TYPE_MULTIPLICATION;
    if (s == "/") return Operation::OPERATION_TYPE_DIVISION;
    if (s == "^") return Operation::OPERATION_TYPE_POWER;
    if (s == "%") return Operation::OPERATION_TYPE_MOD;

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


Operation::Operation(Scope* scope, std::vector<std::string>* contents, OperationType operationType) {
    this->mFirstValue = scope->parseValue(contents->at(0));
    this->mLastValue = scope->parseValue(contents->at(2));

    this->mOperationType = operationType;

}

Operation::Operation(Scope* scope, std::vector<std::string>* contents, std::string operationTypeString) {
    this->mFirstValue = scope->parseValue(contents->at(0));
    this->mLastValue = scope->parseValue(contents->at(1));

    this->mOperationType = Operation::parseOperationType(operationTypeString);
}

Operation::Operation(Scope* scope, std::vector<std::string>* contents) {
    this->mScope = scope;
    this->mFirstValue = scope->parseValue(contents->at(0));
    this->mLastValue = scope->parseValue(contents->at(2));

    this->mOperationType = Operation::parseOperationType(contents->at(1));
}

Operation::~Operation() {
    delete this->mFirstValue;
    delete this->mLastValue;
}

Value* Operation::runAdditionOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getIntValue() + this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getIntValue() + this->mLastValue->getFloatValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getFloatValue() + this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getFloatValue() + this->mLastValue->getFloatValue());
    }

    return new Value(this->mFirstValue->getAsString() + this->mLastValue->getAsString());
}

Value* Operation::runSubstractionOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getIntValue() - this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getIntValue() - this->mLastValue->getFloatValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getFloatValue() - this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getFloatValue() - this->mLastValue->getFloatValue());
    }

    return new Value();
}

Value* Operation::runMultiplicationOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getIntValue() * this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getIntValue() * this->mLastValue->getFloatValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getFloatValue() * this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getFloatValue() * this->mLastValue->getFloatValue());
    }

    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_STRING && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(StringUtils::duplicateWithCount(this->mFirstValue->getStringValue(), this->mLastValue->getIntValue()));
    }

    return new Value();
}

Value* Operation::runDivisionOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getIntValue() / this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getIntValue() / this->mLastValue->getFloatValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getFloatValue() / this->mLastValue->getIntValue());
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value(this->mFirstValue->getFloatValue() / this->mLastValue->getFloatValue());
    }

    return new Value();
}

Value* Operation::runPowerOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value((size_t) std::pow(this->mFirstValue->getIntValue(), this->mLastValue->getIntValue()));
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value((size_t) std::pow(this->mFirstValue->getIntValue(), this->mLastValue->getFloatValue()));
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value((size_t) std::pow(this->mFirstValue->getFloatValue(), this->mLastValue->getIntValue()));
    }
    else if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_FLOAT && this->mLastValue->getValueType() == Value::VALUE_TYPE_FLOAT) {
        return new Value((size_t) std::pow(this->mFirstValue->getFloatValue(), this->mLastValue->getFloatValue()));
    }

    return new Value();
}

Value* Operation::runModOperation() {
    if (this->mFirstValue->getValueType() == Value::VALUE_TYPE_INT && this->mLastValue->getValueType() == Value::VALUE_TYPE_INT) {
        return new Value(this->mFirstValue->getIntValue() % this->mLastValue->getIntValue());
    }

    return new Value();
}

Value* Operation::run() {
    switch (this->mOperationType) {
        case Operation::OPERATION_TYPE_ADDITION: return this->runAdditionOperation();
        case Operation::OPERATION_TYPE_SUBSTRACTION: return this->runSubstractionOperation();
        case Operation::OPERATION_TYPE_MULTIPLICATION: return this->runMultiplicationOperation();
        case Operation::OPERATION_TYPE_DIVISION: return this->runDivisionOperation();
        case Operation::OPERATION_TYPE_POWER: return this->runPowerOperation();
        case Operation::OPERATION_TYPE_MOD: return this->runModOperation();
        default: return new Value();
    }
}