#include "expression.h"

#include <string>
#include <vector>

#include "options.h"
#include "utils.h"
#include "stringutils.h"
#include "operation.h"
#include "scope.h"
#include "value.h"

Value* splitAndFindValue(Scope* scope, std::string content, std::vector<std::string>* delimeters) {
    std::vector<std::string>* values = StringUtils::split(content, delimeters->at(0));

    if (delimeters->size() > 1) {
        std::string delimeter = delimeters->at(0);
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

        // delete &delimeter;
        delete values;

        return resultValue;
    }
    else if (values->size() == 1) {
        std::string item = values->at(0);

        delete values;

        return scope->parseValue(item);
    }
    else {
        auto operation = new Operation(scope, values);

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

        std::string content = StringUtils::substring(this->mContent, index - 1, lastIndex); // 1 => to remove around paranthesis

        Expression* expression = new Expression(this->mScope, content);

        this->mContent = StringUtils::replaceMiddle(this->mContent, expression->run()->getAsString(true), index, lastIndex + 1);

        delete expression;
    }

    auto delimeters = new std::vector<std::string>;
    delimeters->push_back("-");
    delimeters->push_back("+");
    delimeters->push_back("%");
    delimeters->push_back("/");
    delimeters->push_back("*");
    delimeters->push_back("^");

    return splitAndFindValue(this->mScope, this->mContent, delimeters);
}