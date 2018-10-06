#include "expression.h"

#include <string>
#include <vector>

#include "options.h"
#include "utils.h"
#include "stringutils.h"
#include "operation.h"
#include "scope.h"
#include "value.h"
#include "logger.h"

bool canRunOperatorByTurn(std::string s, std::string o) {
}

Value* splitAndFindValue(Scope* scope, std::string content, std::vector<std::string>* delimeters) {
    if (delimeters->empty()) {
        delete delimeters;

        return scope->parseValue(content);
    }

    std::string delimeter = delimeters->at(0);
    std::vector<std::string>* values = StringUtils::split(content, delimeter);

    if (delimeters->size() > 1) {
        Value* resultValue = nullptr;

        for (size_t i = 0; i < values->size(); i++) {
            Value* value = splitAndFindValue(scope, values->at(i), new std::vector<std::string>(delimeters->begin()+1, delimeters->end()));

            if (resultValue == nullptr) {
                resultValue = value;
            }
            else {
                auto operation = new Operation(scope, resultValue, value, delimeter);

                resultValue = operation->run();

                delete operation;
            }
        }

        delete values;

        return resultValue;
    }
    else if (values->size() == 1) {
        std::string item = values->at(0);

        delete values;

        return scope->parseValue(item);
    }
    else {
        auto operation = new Operation(scope, values, delimeter);

        auto value = operation->run();

        delete operation;

        return value;
    }
}

Expression::Expression(Scope* scope, std::string content) {
    this->mScope = scope;
    this->mContent = content;
}

Expression::~Expression() {

}

Value* Expression::run() {
    size_t index, lastIndex;

    while((index = this->mContent.find(Options::START_PARENTHESIS_CHAR)) != std::string::npos) {
        lastIndex = Utils::findSiblingPosition(this->mContent, index, Options::START_PARENTHESIS_CHAR, Options::END_PARENTHESIS_CHAR);

        std::string content = StringUtils::substring(this->mContent, index + 1, lastIndex); // 1 => to remove around paranthesis

        Expression* expression = new Expression(this->mScope, content);

        this->mContent = StringUtils::replaceMiddle(this->mContent, expression->run()->getAsString(true), index, lastIndex + 1);

        delete expression;
    }

    auto delimeters = new std::vector<std::string>;

    if (this->mContent.find('-') != std::string::npos) {
        delimeters->push_back("-");
    }
    if (this->mContent.find('+') != std::string::npos) {
        delimeters->push_back("+");
    }
    if (this->mContent.find('%') != std::string::npos) {
        delimeters->push_back("%");
    }
    if (this->mContent.find('/') != std::string::npos) {
        delimeters->push_back("/");
    }
    if (this->mContent.find('*') != std::string::npos) {
        delimeters->push_back("*");
    }
    if (this->mContent.find('^') != std::string::npos) {
        delimeters->push_back("^");
    }

    auto value = splitAndFindValue(this->mScope, this->mContent, delimeters);

    return value;
}