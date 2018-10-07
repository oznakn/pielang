#include "variable.h"

#include <string.h>

#include "definitions.h"
#include "value.h"
#include "stringutils.h"

bool Variable::isValidVariableName(std::string s) {
    s = StringUtils::trim(s);

    if (s.length() > 0) {
        if (!std::isalpha(s.at(0))) {
            return false;
        }
        else if (s.length() > 1) {
            for (size_t i = 1; i < s.length(); i++) {
                if (!std::isalnum(s.at(i)) && s.at(i) != '_' && s.at(i) != '.') return false; // TODO
            }
        }
    }
    else {
        return false;
    }

    return true;
}

Variable::Variable(std::string variableName, Value* value) {
    this->mVariableName = variableName;
    this->mValue = value;

    this->mValue->linkWithVariable(this);

    this->mLinkedObjectList = new ObjectList;
}

Variable::~Variable() {
    this->mValue->unlinkWithVariable(this);

    delete this->mLinkedObjectList;
}

Value* Variable::getValue() {
    return this->mValue;
}

void Variable::changeValue(Value* value) {
    this->mValue->unlinkWithVariable(this);

    this->mValue = value;

    this->mValue->linkWithVariable(this);
}

void Variable::linkWithObject(Object* object) {
    if (this->mLinkedObjectList == nullptr) {
        this->mLinkedObjectList = new ObjectList;
    }

    this->mLinkedObjectList->push_back(object);
}

void Variable::unlinkWithObject(Object* object) {
    if (this->mLinkedObjectList == nullptr) {
        for (size_t i = 0; i < this->mLinkedObjectList->size(); i++) {
            if (this->mLinkedObjectList->at(i) == object) {
                this->mLinkedObjectList->erase(this->mLinkedObjectList->begin() + i);
                break;
            }
        }

        this->deleteIfNotLinked();
    }
}

void Variable::deleteIfNotLinked() {
    if (this->mLinkedObjectList == nullptr || this->mLinkedObjectList->empty()) {
        delete this;
    }
}