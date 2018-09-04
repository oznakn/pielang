#ifndef PILANG_SCOPE_H
#define PILANG_SCOPE_H

#include <string>
#include <unordered_map>

#include "variable.h"
#include "function.h"

typedef std::unordered_map<std::string, Variable*> VariableMap;
typedef std::unordered_map<std::string, Function*> FunctionMap;

class Scope {
private:
    // std::string mInitialContent;
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
    Scope(std::string, VariableMap*, FunctionMap*);

    VariableMap* getVariableMap();
    FunctionMap* getFunctionMap();

    void setAsMainScope();
    void run();
};


#endif // PILANG_SCOPE_H
