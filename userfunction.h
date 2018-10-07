#ifndef PILANG_USERFUNCTION_H
#define PILANG_USERFUNCTION_H

#include <string>
#include <vector>

#include "definitions.h"
#include "function.h"

class UserFunction : public Function {
private:
    std::string mFunctionName;
    std::string mContent;
    Scope* mScope;
    StringList* mParameterList;

    void init(std::string, std::string, StringList*, Scope*);

public:
    UserFunction(std::string, Scope*);
    UserFunction(std::string, std::string, StringList*, Scope*);
    ~UserFunction();

    std::string getFunctionName();
    Value* run(ValueList*);
};

#endif //PILANG_USERFUNCTION_H
