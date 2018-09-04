#include "scope.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "options.h"
#include "stringutils.h"
#include "utils.h"
#include "stringliteral.h"
#include "logger.h"

Scope::Scope(std::string content) : Scope(content, nullptr, nullptr) {}

Scope::Scope(std::string content, VariableMap* inheritedVariableMap, FunctionMap* inheritedFunctionMap) {
    // this->mInitialContent = content;
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

VariableMap* Scope::getVariableMap() {
    return this->mVariableMap;
}

FunctionMap* Scope::getFunctionMap() {
    return this->mFunctionMap;
}

void Scope::setAsMainScope() {
    this->mIsMainScope = true;
}

void Scope::findAndReplaceStringLiterals() {
    size_t firstIndex = std::string::npos;
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
    size_t lastIndex = 0;

    while ((index = this->mContent.find(Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING, lastIndex)) != std::string::npos) {
        std::string line = StringUtils::trim(StringUtils::substring(this->mContent, lastIndex, index));
        // Logger::debug("Scope", "Executing Line: " + line);

        if (lastIndex == index) {
            lastIndex++;
            continue;
        } else if (line.find(Options::FUNCTION_WORD_STRING) == 0) {
            this->findAndReplaceFirstFunction(lastIndex);
            continue;
        } else if ((tempIndex = line.find(Options::EQUALITY_CHAR)) != std::string::npos &&
            Variable::isValidVariableName((tempString = StringUtils::substring(line, 0, tempIndex)))) {

        }

        this->mContent = StringUtils::replaceMiddle(this->mContent, "", lastIndex - 1, index + 1);
        Logger::debug("Scope", "Execution Result: " + this->mContent);

        lastIndex = index + 1;
    }
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