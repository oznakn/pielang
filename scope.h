#ifndef PILANG_SCOPE_H
#define PILANG_SCOPE_H

#include <string>
#include <vector>
#include <unordered_map>

class Value;
class Variable;
class Function;

typedef std::unordered_map<std::string, Variable*> VariableMap;
typedef std::unordered_map<std::string, Function*> FunctionMap;

class Scope {
private:
    Scope* mInheritedScope = nullptr;
    std::string mContent;

    VariableMap* mVariableMap;
    FunctionMap* mFunctionMap;

    bool mIsMainScope = false;

    void findAndReplaceMultipleSpacesWithOne();
    void findAndReplaceStringLiterals();
    void findAndDeleteComments();
    void combineLines();

    void findAndReplaceFirstFunction(size_t);
    void runLines();

public:
    Scope(std::string);
    Scope(std::string, Scope*);
    ~Scope();

    VariableMap* getVariableMap();
    FunctionMap* getFunctionMap();

    Value* parseValue(std::string);
    void setAsMainScope();
    void run();

    Variable* getVariable(std::string);
    bool hasVariable(std::string);
    void addVariable(std::string, Variable*);
    void removeVariable(std::string);

    Function* getFunction(std::string);
    bool hasFunction(std::string);
    void addFunction(std::string, Function*);
    void removeFunction(std::string);
};


#endif // PILANG_SCOPE_H
