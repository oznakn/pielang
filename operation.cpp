#include "operation.h"

#include <string>
#include <vector>

#include "scope.h"
#include "operator.h"
#include "value.h"
#include "variable.h"
#include "stringutils.h"

Operation::Operation(Scope* scope, Value* firstValue, Value* secondValue, Operator* anOperator) {
    this->mScope = scope;
    this->mFirstValue = firstValue;
    this->mSecondValue = secondValue;
    this->mOperator = anOperator;
}

Operation::Operation(Scope* scope, Value* firstValue, Operator* anOperator) {
    this->mScope = scope;
    this->mFirstValue = firstValue;
    this->mOperator = anOperator;
}

Operation::~Operation() {
    if (this->mFirstValue != nullptr) this->mFirstValue->deleteIfNotLinked();

    if (this->mSecondValue != nullptr) this->mSecondValue->deleteIfNotLinked();
}

Value* Operation::run() {
    return this->mOperator->run(this->mFirstValue, this->mSecondValue);
}