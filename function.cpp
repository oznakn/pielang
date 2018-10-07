#include "function.h"

#include <string>
#include <vector>
#include <iostream>

#include "definitions.h"
#include "value.h"

Function::~Function() {
    delete this->mLinkedObjectList;
}

std::string Function::getFunctionName() {
    return "undefined";
}

Value* Function::run(std::vector<Value *> *) {
    return Value::undefined;
}

const void Function::linkWithObject(Object* object) {
    if (this->mLinkedObjectList == nullptr) {
        this->mLinkedObjectList = new ObjectList;
    }

    this->mLinkedObjectList->push_back(object);
}

const void Function::unlinkWithObject(Object* object) {
    if (this->mLinkedObjectList != nullptr) {
        for (size_t i = 0; i < this->mLinkedObjectList->size(); i++) {
            if (this->mLinkedObjectList->at(0) == object) {
                this->mLinkedObjectList->erase(this->mLinkedObjectList->begin() + i);

                break;
            }
        }

        this->deleteIfNotLinked();
    }
}

const void Function::deleteIfNotLinked() {
    if (this->mLinkedObjectList == nullptr || this->mLinkedObjectList->empty()) {
        delete this;
    }
}
