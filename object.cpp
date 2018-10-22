#include "object.h"

#include <string>
#include <vector>

#include "definitions.h"
#include "stringutils.h"
#include "value.h"
#include "function.h"
#include "variable.h"
#include "systemfunction.h"

Object::Object() {
    this->mVariableMap = new VariableMap;
    this->mFunctionMap = new FunctionMap;
    this->mObjectMap = new ObjectMap;
}

Object::Object(std::string objectName) {
    this->mObjectName = objectName;

    this->mVariableMap = new VariableMap;
    this->mFunctionMap = new FunctionMap;
    this->mObjectMap = new ObjectMap;
}

Object::~Object() {
    for (std::pair<std::string, Variable*> element : *this->mVariableMap) {
        element.second->unlinkWithObject(this);
    }

    for (std::pair<std::string, Function*> element : *this->mFunctionMap) {
        element.second->unlinkWithObject(this);
    }

    for (std::pair<std::string, Object*> element : *this->mObjectMap) {
        element.second->unlinkWithObject(this);
    }

    delete this->mVariableMap;
    delete this->mFunctionMap;
    delete this->mObjectMap;
}

void Object::linkWithObject(Object* object) {
    if (this->mLinkedObjectList == nullptr) {
        this->mLinkedObjectList = new ObjectList;
    }

    this->mLinkedObjectList->push_back(object);
}

void Object::unlinkWithObject(Object* object) {
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

void Object::deleteIfNotLinked() {
    if (this->mLinkedObjectList == nullptr || this->mLinkedObjectList->empty()) {
        delete this;
    }
}

Object* Object::getWorkingObject(std::string& dotNotation) {
    if (StringUtils::count(dotNotation, ".") == 0) return this;

    auto workingObject = this;

    StringList* stringList = StringUtils::split(dotNotation, ".");

    for (size_t i = 0; i < stringList->size() - 1; i++) {
        workingObject = workingObject->getObject(stringList->at(0));
    }

    return workingObject;
}

std::string Object::getRealNameFromDotNotation(std::string& dotNotation) {
    size_t index = dotNotation.find_last_of('.');

    if (index == std::string::npos) return dotNotation;

    return StringUtils::substring(dotNotation, index+1, dotNotation.size());
}

VariableMap* Object::getVariableMap() {
    return this->mVariableMap;
}

FunctionMap* Object::getFunctionMap() {
    return this->mFunctionMap;
}

Variable* Object::getVariable(std::string variableName) {
    return this->getWorkingObject(variableName)->mVariableMap->at(this->getRealNameFromDotNotation(variableName));
}

bool Object::hasVariable(std::string variableName) {
    auto workingObject = this->getWorkingObject(variableName);

    return workingObject->mVariableMap->find(this->getRealNameFromDotNotation(variableName)) != workingObject->mVariableMap->end();
}

void Object::addVariable(std::string variableName, Variable* variable) {
    this->mVariableMap->insert(std::make_pair(variableName, variable));
    variable->linkWithObject(this);
}

void Object::removeVariable(std::string variableName) {
    if (this->hasVariable(variableName)) {
        this->mVariableMap->at(variableName)->unlinkWithObject(this);
        this->mVariableMap->erase(variableName);
    }
}

Variable* Object::createVariable(std::string variableName, Value* value) {
    auto variable = new Variable(variableName, value);

    this->addVariable(variableName, variable);
    variable->linkWithObject(this);

    return variable;
}

Function* Object::getFunction(std::string functionName) {
    return this->getWorkingObject(functionName)->mFunctionMap->at(this->getRealNameFromDotNotation(functionName));
}

bool Object::hasFunction(std::string functionName) {
    auto workingObject = this->getWorkingObject(functionName);
    return workingObject->mFunctionMap->find(this->getRealNameFromDotNotation(functionName)) != workingObject->mFunctionMap->end();
}

void Object::addFunction(std::string functionName, Function* function) {
    this->mFunctionMap->insert(std::make_pair(functionName, function));
    function->linkWithObject(this);
}

void Object::removeFunction(std::string functionName) {
    if (this->hasFunction(functionName)) {
        this->mFunctionMap->at(functionName)->unlinkWithObject(this);
        this->mFunctionMap->erase(functionName);
    }
}

Function* Object::createSystemFunction(std::string functionName, FunctionCallback functionCallback) {
    auto systemFunction = new SystemFunction(functionName, functionCallback);

    this->addFunction(functionName, systemFunction);

    return systemFunction;
}

Object* Object::getObject(std::string objectName) {
    return this->mObjectMap->at(objectName);
}

bool Object::hasObject(std::string objectName) {
    return this->mObjectMap->find(objectName) != this->mObjectMap->end();
}

void Object::addObject(std::string objectName, Object* object) {
    this->mObjectMap->insert(std::make_pair(objectName, object));
    object->linkWithObject(this);
}

void Object::removeObject(std::string objectName) {
    if (this->hasObject(objectName)) {
        this->mObjectMap->at(objectName)->unlinkWithObject(this);
        this->mObjectMap->erase(objectName);
    }
}

Object* Object::createObject(std::string objectName) {
    auto object = new Object(objectName);

    this->addObject(objectName, object);
    object->linkWithObject(this);

    return object;
}

