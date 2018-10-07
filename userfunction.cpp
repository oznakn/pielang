#include "userfunction.h"

#include <string>
#include <vector>

#include "options.h"
#include "scope.h"
#include "value.h"
#include "stringutils.h"
#include "logger.h"
#include "variable.h"

UserFunction::UserFunction(std::string outerContent, Scope *inheritedScope) {
    size_t endOfFunctionWordIndex = outerContent.find(Options::FUNCTION_WORD_STRING);
    size_t firstParametersContentIndex = outerContent.find(Options::START_PARENTHESIS_CHAR);
    size_t firstFunctionContentIndex = outerContent.find(Options::START_BLOCK_CHAR);
    size_t lastFunctionContentIndex;

    std::string functionName = StringUtils::trim(StringUtils::substring(outerContent, endOfFunctionWordIndex + Options::FUNCTION_WORD_STRING.length(), firstParametersContentIndex));

    std::string parametersString = StringUtils::trim(StringUtils::substring(outerContent, firstParametersContentIndex, firstFunctionContentIndex));
    parametersString = StringUtils::trim(StringUtils::substring(parametersString, 1, parametersString.length() - 1));

    std::vector<std::string>* parameterList = StringUtils::split(parametersString, ",");

    for (size_t i = 0; i < parameterList->size(); i++) {
        parameterList->at(i) = StringUtils::trim(parameterList->at(i));
    }

    lastFunctionContentIndex = outerContent.find(Options::END_BLOCK_CHAR, firstFunctionContentIndex);

    std::string content = StringUtils::substring(outerContent, firstFunctionContentIndex + 1, lastFunctionContentIndex); // 1 => Options::START_BLOCK_CHAR

    this->init(content, functionName, parameterList, inheritedScope);
}

UserFunction::UserFunction(std::string content, std::string functionName, std::vector<std::string>* parameterList, Scope *inheritedScope) {
    this->init(content, functionName, parameterList, inheritedScope);
}

UserFunction::~UserFunction() {
    delete this->mParameterList;
}

void UserFunction::init(std::string content, std::string functionName, std::vector<std::string>* parameterList, Scope *inheritedScope) {
    this->mContent = content;
    this->mFunctionName = functionName;
    this->mParameterList = parameterList;

    this->mScope = new Scope(this->mContent, inheritedScope);
}

std::string UserFunction::getFunctionName() {
    return this->mFunctionName;
}

Value* UserFunction::run(std::vector<Value*>* argumentList) {
    for (size_t i = 0; i < argumentList->size(); i++) {
        auto variableName = this->mParameterList->at(i);
        this->mScope->addVariable(variableName, new Variable(variableName, argumentList->at(i)));
    }

    this->mScope->run();

    for (size_t i = 0; i < argumentList->size(); i++) {
        this->mScope->removeVariable(this->mParameterList->at(i));
    }

    return nullptr;
}

size_t UserFunction::getParameterCount() {
    return this->mParameterList->size();
}