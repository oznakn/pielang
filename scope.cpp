#include "scope.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "definitions.h"
#include "options.h"
#include "stringutils.h"
#include "utils.h"
#include "stringliteral.h"
#include "logger.h"
#include "function.h"
#include "userfunction.h"
#include "variable.h"
#include "value.h"
#include "object.h"
#include "expression.h"
#include "system.h"
#include "systemfunction.h"

Scope::Scope(std::string content) : Scope(content, nullptr) {}

Scope::Scope(std::string content, Scope* inheritedScope) {
    this->mContent = content;

    this->mScopeObject = new Object("__main__");

    if (inheritedScope != nullptr) {
        this->mInheritedScope = inheritedScope;
    }

    System::initForScope(this);
}

Scope::~Scope() {
    delete this->mScopeObject;
}

void Scope::setScopeResult(Value* value) {
    this->mScopeResult = value;
}

Value* Scope::getScopeResult() {
    return this->mScopeResult;
}

VariableMap* Scope::getVariableMap() {
    return this->mScopeObject->getVariableMap();
}

FunctionMap* Scope::getFunctionMap() {
    return this->mScopeObject->getFunctionMap();
}

void Scope::setAsMainScope() {
    this->mIsMainScope = true;
}

void Scope::findAndReplaceStringLiterals() {
    size_t firstIndex;
    size_t lastIndex;

    const size_t stringCharLength = 1;
    const size_t escapeCharLength = 1;

    while ((firstIndex = this->mContent.find(Options::STRING_CHAR)) != std::string::npos) {
        size_t additionalPosition = 0;
        lastIndex = this->mContent.find(Options::STRING_CHAR, firstIndex + additionalPosition + stringCharLength);

        while (lastIndex != std::string::npos && this->mContent.at(lastIndex - 1) == Options::STRING_ESCAPE_CHAR) {
            this->mContent.replace(lastIndex - 1, escapeCharLength, "");

            lastIndex = this->mContent.find(Options::STRING_CHAR, lastIndex);
        }

        if (lastIndex != std::string::npos) {
            std::string middle = StringUtils::substring(this->mContent, firstIndex + stringCharLength, lastIndex);

            StringLiteral* stringLiteral = new StringLiteral(middle);

            this->mContent = StringUtils::replaceMiddle(this->mContent, stringLiteral->getAsString(), firstIndex, lastIndex + stringCharLength);
        }
    }
}

void Scope::findAndReplaceMultipleSpacesWithOne() {
    this->mContent = StringUtils::replaceMultipleSpacesWithOne(this->mContent);
}

void Scope::findAndDeleteComments() {
    size_t firstIndex;
    size_t lastIndex;

    size_t startStringLength = Options::MULTI_COMMENT_START_STRING.length();
    size_t endStringLength = Options::MULTI_COMMENT_END_STRING.length();

    while ((firstIndex = this->mContent.find(Options::MULTI_COMMENT_START_STRING)) != std::string::npos) {
        lastIndex = this->mContent.find(Options::MULTI_COMMENT_END_STRING, firstIndex + startStringLength);

        if (lastIndex != std::string::npos) {
            this->mContent = StringUtils::cutMiddle(this->mContent, firstIndex, lastIndex + endStringLength);
        }
    }

    startStringLength = Options::COMMENT_START_STRING.length();
    endStringLength = Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING.length();

    while ((firstIndex = this->mContent.find(Options::COMMENT_START_STRING)) != std::string::npos) {
        lastIndex = this->mContent.find(Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING, firstIndex + startStringLength);

        if (lastIndex != std::string::npos) {
            this->mContent = StringUtils::cutMiddle(this->mContent, firstIndex, lastIndex + endStringLength);
        }
    }
}

void Scope::combineLines() {
    size_t index;

    const size_t endOfLineStringLength = Options::END_OF_LINE.length();
    const size_t endOfLineWithOptionalCharStringLength = Options::END_OF_LINE_WITH_OPTIONAL_CHAR.length();
    const size_t windowsEndOfLineStringLength = Options::WINDOWS_END_OF_LINE.length();
    const size_t windowsEndOfLineWithOptionalCharStringLength = Options::WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR.length();
    const size_t classicMacEndOfLineStringLength = Options::CLASSIC_MAC_END_OF_LINE.length();
    const size_t classicMacEndOfLineWithOptionalCharStringLength = Options::CLASSIC_MAC_END_OF_LINE_WITH_OPTIONAL_CHAR.length();

    while ((index = this->mContent.find(Options::WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR)) != std::string::npos) {
        this->mContent.replace(index, windowsEndOfLineWithOptionalCharStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::CLASSIC_MAC_END_OF_LINE_WITH_OPTIONAL_CHAR)) != std::string::npos) {
        this->mContent.replace(index, classicMacEndOfLineWithOptionalCharStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::END_OF_LINE_WITH_OPTIONAL_CHAR)) != std::string::npos) {
        this->mContent.replace(index, endOfLineWithOptionalCharStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::WINDOWS_END_OF_LINE)) != std::string::npos) {
        this->mContent.replace(index, windowsEndOfLineStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::CLASSIC_MAC_END_OF_LINE)) != std::string::npos) {
        this->mContent.replace(index, classicMacEndOfLineStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::END_OF_LINE)) != std::string::npos) {
        this->mContent.replace(index, endOfLineStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    this->mContent = StringUtils::replaceMultipleCharsWithOne(this->mContent, Options::END_OF_LINE_OPTIONAL_CHAR);

    if (this->mContent.at(this->mContent.length() - 1) != Options::END_OF_LINE_OPTIONAL_CHAR) {
        this->mContent += Options::END_OF_LINE_OPTIONAL_CHAR;
    }
}

void Scope::findAndReplaceFirstFunction(size_t startIndex) {
    size_t startBlockIndex, endBlockIndex;

    startBlockIndex = this->mContent.find(Options::START_BLOCK_CHAR, startIndex);
    endBlockIndex = Utils::findSiblingPosition(this->mContent, startBlockIndex, Options::START_BLOCK_CHAR, Options::END_BLOCK_CHAR);

    auto function = new UserFunction(StringUtils::substring(this->mContent, startIndex, endBlockIndex), this);

    this->mScopeObject->addFunction(function->getFunctionName(), function);

    this->mContent = StringUtils::replaceMiddle(this->mContent, "", startIndex, endBlockIndex + 1); // 1 => END_BLOCK_CHAR
}

void Scope::runLines() {
    size_t index, tempIndex;
    std::string tempString;

    while ((index = this->mContent.find(Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING, 0)) != std::string::npos) {
        std::string line = StringUtils::trim(StringUtils::substring(this->mContent, 0, index));

        if (line.find(Options::FUNCTION_WORD_STRING) == 0) {
            this->findAndReplaceFirstFunction(0);
            continue;
        }
        else {
            auto expression = new Expression(this, line);
            expression->run();
        }

        this->mContent = StringUtils::replaceMiddle(this->mContent, "", 0, index + 1);
    }
}

bool Scope::isValueParseable(std::string s) {
    s = StringUtils::trim(s);

    return (Variable::isValidVariableName(s) && this->hasVariable(s)) ||
        (StringLiteral::isValidStringLiteral(s)) ||
        (Value::isParseable(s));
}

Value* Scope::parseValue(std::string s) {
    s = StringUtils::trim(s);
    Value* value = nullptr;

    if (Variable::isValidVariableName(s) && this->hasVariable(s)) {
        value = this->getVariable(s)->getValue();
    }
    else if (StringLiteral::isValidStringLiteral(s)) {
        value = StringLiteral::findStringLiteral(s)->getValue();
    }
    else if (Value::isParseable(s)) {
        value = Value::parseStringToValue(s);
    }

    if (value == nullptr) {
        value = new Value; // TODO
    }

    value->setRepresentation(s);

    return value;
}

void Scope::run() {
    if (this->mIsMainScope) {
        this->findAndReplaceStringLiterals();
        this->findAndReplaceMultipleSpacesWithOne();
        this->combineLines();
        this->findAndDeleteComments();
    }

    this->runLines();
}

bool Scope::hasVariable(std::string variableName) {
    return this->mScopeObject->hasVariable(variableName) ||
        (this->mInheritedScope != nullptr && this->mInheritedScope->hasVariable(variableName));
}

Variable* Scope::getVariable(std::string variableName) {
    if (this->mScopeObject->hasVariable(variableName)) {
        return this->mScopeObject->getVariable(variableName);
    }
    else if (this->mInheritedScope != nullptr && this->mInheritedScope->getVariableMap()->find(variableName) != this->mInheritedScope->getVariableMap()->end()) {
        return this->mInheritedScope->getVariableMap()->at(variableName);
    }

    return nullptr; // TODO
}

void Scope::addVariable(std::string variableName, Variable* variable) {
    this->mScopeObject->addVariable(variableName, variable);
}

void Scope::removeVariable(std::string variableName) {
    this->mScopeObject->removeVariable(variableName);
}

Variable* Scope::createVariable(std::string variableName, Value* value) {
    return this->mScopeObject->createVariable(variableName, value);
}

bool Scope::hasFunction(std::string functionName) {
    return this->mScopeObject->hasFunction(functionName) ||
        (this->mInheritedScope != nullptr && this->mInheritedScope->hasFunction(functionName));
}

Function* Scope::getFunction(std::string functionName) {
    if (this->mScopeObject->hasFunction(functionName)) {
        return this->mScopeObject->getFunction(functionName);
    }

    if (this->mInheritedScope != nullptr && this->mInheritedScope->getFunctionMap()->find(functionName) != this->mInheritedScope->getFunctionMap()->end()) {
        return this->mInheritedScope->getFunctionMap()->at(functionName);
    }

    return nullptr;
}

void Scope::addFunction(std::string functionName, Function* function) {
    this->mScopeObject->addFunction(functionName, function);
}

void Scope::removeFunction(std::string functionName) {
   this->mScopeObject->removeFunction(functionName);
}

Function* Scope::createSystemFunction(std::string functionName, FunctionCallback functionCallback) {
    return this->mScopeObject->createSystemFunction(functionName, functionCallback);
}

Object* Scope::getObject(std::string objectName) {
    return this->mScopeObject->getObject(objectName);
}

bool Scope::hasObject(std::string objectName) {
    return this->mScopeObject->hasObject(objectName);
}

void Scope::addObject(std::string objectName, Object* object) {
    this->mScopeObject->addObject(objectName, object);
}

void Scope::removeObject(std::string objectName) {
    if (this->hasObject(objectName)) {
        this->mScopeObject->removeObject(objectName);
    }
}

Object* Scope::createObject(std::string objectName) {
    return this->mScopeObject->createObject(objectName);
}

