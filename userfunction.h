#ifndef PILANG_USERFUNCTION_H
#define PILANG_USERFUNCTION_H

#include <string>
#include <vector>

#include "function.h"

class Scope;
class Value;
class Function;

class UserFunction : public Function {
private:
    std::string mFunctionName;
    std::string mContent;
    Scope* mScope;
    std::vector<std::string>* mParameterList;

    void init(std::string, std::string, std::vector<std::string>*, Scope*);

public:
    UserFunction(std::string, Scope*);
    UserFunction(std::string, std::string, std::vector<std::string>*, Scope*);
    ~UserFunction();

    std::string getFunctionName();
    Value* run(std::vector<Value*>*);
    size_t getParameterCount();
};

#endif //PILANG_USERFUNCTION_H
