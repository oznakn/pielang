#include "scope.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "options.h"
#include "stringutils.h"
#include "utils.h"
#include "stringliteral.h"
#include "logger.h"

using namespace std;

Scope::Scope(string content) : Scope(content, nullptr, nullptr) {}

Scope::Scope(string content, VariableMap* inheritedVariableMap, FunctionMap* inheritedFunctionMap) {
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
    size_t firstIndex = string::npos;
    size_t lastIndex = string::npos;

    const size_t stringCharLength = 1;
    const size_t escapeCharLength = 1;

    while ((firstIndex = this->mContent.find(Options::STRING_CHAR, lastIndex + stringCharLength)) != string::npos) {
        size_t additionalPosition = 0;
        lastIndex = this->mContent.find(Options::STRING_CHAR, firstIndex + additionalPosition + stringCharLength);

        while (lastIndex != string::npos && this->mContent.at(lastIndex-1) == Options::STRING_ESCAPE_CHAR) {
            additionalPosition += stringCharLength + (escapeCharLength*2);

            this->mContent.replace(lastIndex-1, escapeCharLength, "");

            lastIndex = this->mContent.find(Options::STRING_CHAR, firstIndex + additionalPosition + stringCharLength);
        }

        if (lastIndex != string::npos) {
            string middle = StringUtils::substring(this->mContent, firstIndex + stringCharLength, lastIndex);

            StringLiteral* stringLiteral = new StringLiteral(middle);

            this->mContent = StringUtils::replaceMiddle(this->mContent, stringLiteral->getAsString(), firstIndex, lastIndex + stringCharLength);
        }
    }
}

void Scope::findAndDeleteComments() {
    size_t firstIndex = string::npos;
    size_t lastIndex = string::npos;

    const size_t startStringLength = Options::MULTI_COMMENT_START_STRING.length();
    const size_t endStringLength = Options::MULTI_COMMENT_END_STRING.length();

    while ((firstIndex = this->mContent.find(Options::MULTI_COMMENT_START_STRING)) != string::npos) {
        lastIndex = this->mContent.find(Options::MULTI_COMMENT_END_STRING, firstIndex + startStringLength);

        if (lastIndex != string::npos) {
            this->mContent = StringUtils::cutMiddle(this->mContent, firstIndex, lastIndex + endStringLength);
        }
    }
}

void Scope::combineLines() {
    size_t index = string::npos;

    const size_t endOfLineStringLength = Options::END_OF_LINE.length();
    const size_t endOfLineWithOptionalCharStringLength = Options::END_OF_LINE_WITH_OPTIONAL_CHAR.length();
    const size_t windowsEndOfLineStringLength = Options::WINDOWS_END_OF_LINE.length();
    const size_t windowsEndOfLineWithOptionalCharStringLength = Options::WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR.length();

    while ((index = this->mContent.find(Options::WINDOWS_END_OF_LINE_WITH_OPTIONAL_CHAR)) != string::npos) {
        this->mContent.replace(index, windowsEndOfLineWithOptionalCharStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::END_OF_LINE_WITH_OPTIONAL_CHAR)) != string::npos) {
        this->mContent.replace(index, endOfLineWithOptionalCharStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::WINDOWS_END_OF_LINE)) != string::npos) {
        this->mContent.replace(index, windowsEndOfLineStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

    while ((index = this->mContent.find(Options::END_OF_LINE)) != string::npos) {
        this->mContent.replace(index, endOfLineStringLength, Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING);
    }

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
    size_t index;
    size_t lastIndex = 0;

    while ((index = this->mContent.find(Options::END_OF_LINE_OPTIONAL_CHAR_AS_STRING, lastIndex)) != string::npos) {
        string line = StringUtils::trim(StringUtils::substring(this->mContent, lastIndex, index));

        if (lastIndex == index) {
            lastIndex++;
            continue;
        }

        if (line.find(Options::FUNCTION_WORD_STRING) == 0) {
            this->findAndReplaceFirstFunction(lastIndex);
            continue;
        }

        this->mContent = StringUtils::replaceMiddle(this->mContent, "", lastIndex, index + 1);

        lastIndex = index + 1;
    }
}

void Scope::run() {
    if (this->mIsMainScope) {
        this->findAndReplaceStringLiterals();
        this->findAndDeleteComments();
        this->combineLines();
    }

    Logger::debug("Scope", "Last version of content: " + this->mContent);

    this->runLines();
}