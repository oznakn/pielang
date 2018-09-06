#include "scope.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "options.h"
#include "stringutils.h"
#include "utils.h"
#include "stringliteral.h"
#include "logger.h"
#include "function.h"
#include "variable.h"
#include "value.h"
#include "expression.h"
#include "system.h"
#include "systemfunction.h"

Scope::Scope(std::string content) : Scope(content, nullptr, nullptr) {}

Scope::Scope(std::string content, VariableMap* inheritedVariableMap, FunctionMap* inheritedFunctionMap) {
    this->mContent = content;

    this->mVariableMap = new VariableMap;
    this->mFunctionMap = new FunctionMap;

    if (inheritedVariableMap != nullptr) {
        this->mVariableMap->insert(inheritedVariableMap->begin(), inheritedVariableMap->end());
    }

    if (inheritedFunctionMap != nullptr) {
        this->mFunctionMap->insert(inheritedFunctionMap->begin(), inheritedFunctionMap->end());
    }
}

Scope::~Scope() {
    for (std::pair<std::string, Variable*> element : *this->mVariableMap) {
        delete element.second;
    }

    for (std::pair<std::string, Function*> element : *this->mFunctionMap) {
        delete element.second;
    }

    // delete &this->mContent;
    delete this->mVariableMap;
    delete this->mFunctionMap;
}

VariableMap* Scope::getVariableMap() {
    return this->mVariableMap;
}

FunctionMap* Scope::getFunctionMap() {
    return this->mFunctionMap;
}

void Scope::setAsMainScope() {
    this->mIsMainScope = true;
}

void Scope::runFunction(std::string, std::vector<Value*> argumentList) {

}

void Scope::findAndReplaceStringLiterals() {
    size_t firstIndex;
    size_t lastIndex = std::string::npos;

    const size_t stringCharLength = 1;
    const size_t escapeCharLength = 1;

    while ((firstIndex = this->mContent.find(Options::STRING_CHAR, lastIndex + stringCharLength)) != std::string::npos) {
        size_t additionalPosition = 0;
        lastIndex = this->mContent.find(Options::STRING_CHAR, firstIndex + additionalPosition + stringCharLength);

        while (lastIndex != std::string::npos && this->mContent.at(lastIndex-1) == Options::STRING_ESCAPE_CHAR) {
            additionalPosition += stringCharLength + (escapeCharLength*2);

            this->mContent.replace(lastIndex-1, escapeCharLength, "");

            lastIndex = this->mContent.find(Options::STRING_CHAR, firstIndex + additionalPosition + stringCharLength);
        }

        if (lastIndex != std::string::npos) {
            std::string middle = StringUtils::substring(this->mContent, firstIndex + stringCharLength, lastIndex);

            StringLiteral* stringLiteral = new StringLiteral(new Value(middle));

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

    const size_t startStringLength = Options::MULTI_COMMENT_START_STRING.length();
    const size_t endStringLength = Options::MULTI_COMMENT_END_STRING.length();

    while ((firstIndex = this->mContent.find(Options::MULTI_COMMENT_START_STRING)) != std::string::npos) {
        lastIndex = this->mContent.find(Options::MULTI_COMMENT_END_STRING, firstIndex + startStringLength);

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

    while ((index = this->mContent.find(Options::WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR)) != std::string::npos) {
        this->mContent.replace(index, windowsEndOfLineWithOptionalCharStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::END_OF_LINE_WITH_OPTIONAL_CHAR)) != std::string::npos) {
        this->mContent.replace(index, endOfLineWithOptionalCharStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::WINDOWS_END_OF_LINE)) != std::string::npos) {
        this->mContent.replace(index, windowsEndOfLineStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
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

    Function* function = new Function(StringUtils::substring(this->mContent, startIndex, endBlockIndex), this);

    this->mFunctionMap->insert(make_pair(function->getFunctionName(), function));

    this->mContent = StringUtils::replaceMiddle(this->mContent, "", startIndex, endBlockIndex + 1); // 1 => END_BLOCK_CHAR
}

void Scope::runLines() {
    size_t index, tempIndex;
    std::string tempString;

    while ((index = this->mContent.find(Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING, 0)) != std::string::npos) {
        std::string line = StringUtils::trim(StringUtils::substring(this->mContent, 0, index));
        Logger::debug("Scope", "Exacuting line: " + line);

        if (line.find(Options::FUNCTION_WORD_STRING) == 0) {
            this->findAndReplaceFirstFunction(0);
            continue;
        } else if ((tempIndex = line.find(Options::EQUALITY_CHAR)) != std::string::npos &&
            Variable::isValidVariableName((tempString = StringUtils::substring(line, 0, tempIndex)))) {
            tempString = StringUtils::trim(tempString);

            auto expression = new Expression(this, StringUtils::substring(line, tempIndex + 1, line.length())); // 1 => length of '='
            auto value = expression->run();
            auto variable = new Variable(tempString, value);

            Logger::debug("Scope", "New Variable: " + tempString + " => " + variable->getValue()->getAsString());

            this->mVariableMap->insert(std::make_pair(tempString, variable));
        } else {
            tempIndex = line.find(Options::START_PARENTHESIS_CHAR);
            size_t tempIndex2 = line.find_last_of(Options::END_PARENTHESIS_CHAR);

            auto argumentStringList = StringUtils::split(StringUtils::substring(line, tempIndex + 1, tempIndex2), ",");
            auto argumentList = new std::vector<Value*>;

            for (size_t i = 0; i < argumentStringList->size(); i++) {
                auto expression = new Expression(this, argumentStringList->at(i));
                argumentList->push_back(expression->run());
                delete expression;
            }

            tempString = StringUtils::trim(StringUtils::substring(line, 0, tempIndex));
            Logger::debug("Scope", "tempString: " + tempString);

            if (this->hasFunction(tempString)) {
                auto function = this->getFunction(tempString);
                function->run(argumentList);
            }
            else if (System::hasSystemFunction(tempString)) {
                auto systemFunction = System::getSystemFunction(tempString);
                Logger::debug("Scope", "anan");
                systemFunction->run(argumentList);
            }
        }

        this->mContent = StringUtils::replaceMiddle(this->mContent, "", 0, index + 1);
        Logger::debug("Scope", "After Execution: " + this->mContent);
    }
}

Value* Scope::parseValue(std::string s) {
    s = StringUtils::trim(s);

    if (Variable::isValidVariableName(s) && this->mVariableMap->find(s) != this->mVariableMap->end()) {
        return this->mVariableMap->at(s)->getValue();
    }
    else if (StringLiteral::isValidStringLiteral(s)) {
        return StringLiteral::findStringLiteral(s)->getValue();
    }
    else if (Value::isParseable(s)) {
        return Value::parseStringToValue(s);
    }

    return new Value();
}

void Scope::run() {
    if (this->mIsMainScope) {
        this->findAndReplaceStringLiterals();
        this->findAndReplaceMultipleSpacesWithOne();
        this->findAndDeleteComments();
        this->combineLines();
    }

    Logger::debug("Scope", "Last version of content: " + this->mContent);

    this->runLines();
}


bool Scope::hasVariable(std::string variableName) {
    return this->mVariableMap->find(variableName) != this->mVariableMap->end();
}


Variable* Scope::getVariable(std::string variableName) {
    return this->mVariableMap->at(variableName);
}

void Scope::addVariable(std::string variableName, Variable* variable) {
    this->mVariableMap->insert(std::make_pair(variableName, variable));
}

void Scope::removeVariable(std::string variableName) {
    if (this->mVariableMap->find(variableName) != this->mVariableMap->end()) {
        Variable* variable = this->mVariableMap->at(variableName);

        this->mVariableMap->erase(variableName);

        delete variable;
    }
}

bool Scope::hasFunction(std::string functionName) {
    return this->mFunctionMap->find(functionName) != this->mFunctionMap->end();
}


Function* Scope::getFunction(std::string functionName) {
    return this->mFunctionMap->at(functionName);
}

void Scope::addFunction(std::string functionName, Function* function) {
    this->mFunctionMap->insert(std::make_pair(functionName, function));
}

void Scope::removeFunction(std::string functionName) {
    if (this->mFunctionMap->find(functionName) != this->mFunctionMap->end()) {
        Function* function = this->mFunctionMap->at(functionName);

        this->mFunctionMap->erase(functionName);

        delete function;
    }
}
