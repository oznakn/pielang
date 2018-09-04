#include "function.h"

#include <string>
#include <vector>

#include "options.h"
#include "scope.h"
#include "stringutils.h"
#include "logger.h"

Function::Function(std::string outerContent, Scope *inheritedScope) {
    size_t endOfFunctionWordIndex = outerContent.find(Options::FUNCTION_WORD_STRING);
    size_t firstParametersContentIndex = outerContent.find(Options::START_PARENTHESIS_CHAR);
    size_t firstFunctionContentIndex = outerContent.find(Options::START_BLOCK_CHAR);

    std::string functionName = StringUtils::trim(StringUtils::substring(outerContent, endOfFunctionWordIndex + Options::FUNCTION_WORD_STRING.length(), firstParametersContentIndex));

    std::string parametersString = StringUtils::trim(StringUtils::substring(outerContent, firstParametersContentIndex, firstFunctionContentIndex));
    parametersString = StringUtils::trim(StringUtils::substring(parametersString, 1, parametersString.length() - 1));

    std::vector<std::string>* parameterList = StringUtils::split(parametersString, ",");

    for (size_t i = 0; i < parameterList->size(); i++) {
        parameterList->at(i) = StringUtils::trim(parameterList->at(i));
    }

    size_t lastFunctionContentIndex = outerContent.find(Options::END_BLOCK_CHAR, firstFunctionContentIndex);

    std::string content = StringUtils::substring(outerContent, firstFunctionContentIndex + 1, lastFunctionContentIndex); // 1 => Options::START_BLOCK_CHAR

    this->init(content, functionName, parameterList, inheritedScope);
}

Function::Function(std::string content, std::string functionName, std::vector<std::string>* parameterList, Scope *inheritedScope) {
    this->init(content, functionName, parameterList, inheritedScope);
}

void Function::init(std::string content, std::string functionName, std::vector<std::string>* parameterList, Scope *inheritedScope) {
    this->mContent = content;
    this->mFunctionName = functionName;
    this->mParameterList = parameterList;

    this->mScope = new Scope(this->mContent, inheritedScope->getVariableMap(), inheritedScope->getFunctionMap());
}

std::string Function::getFunctionName() {
    return this->mFunctionName;
}