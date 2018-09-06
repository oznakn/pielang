#ifndef PILANG_FUNCTION_H
#define PILANG_FUNCTION_H

#include <string>
#include <vector>

class Scope;
class Value;

class Function {
private:
    std::string mFunctionName;
    std::string mContent;
    Scope* mScope;
    std::vector<std::string>* mParameterList;

    void init(std::string, std::string, std::vector<std::string>*, Scope*);

public:
    Function(std::string, Scope*);
    Function(std::string, std::string, std::vector<std::string>*, Scope*);

    std::string getFunctionName();
    Value* run(std::vector<Value*>*);
};

#endif //PILANG_FUNCTION_H
