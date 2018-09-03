#ifndef PILANG_SCOPE_H
#define PILANG_SCOPE_H

#include <string>
#include <unordered_map>

#include "variable.h"
#include "function.h"

using namespace std;

typedef unordered_map<string, Variable*> VariableMap;
typedef unordered_map<string, Function*> FunctionMap;

class Scope {
private:
    // string mInitialContent;
    string mContent;

    VariableMap* mVariableMap;
    FunctionMap* mFunctionMap;

    bool mIsMainScope = false;

    void findAndReplaceStringLiterals();
    void findAndDeleteComments();
    void combineLines();

    void findAndReplaceFirstFunction(size_t);
    void runLines();

public:
    Scope(string);
    Scope(string, VariableMap*, FunctionMap*);

    VariableMap* getVariableMap();
    FunctionMap* getFunctionMap();

    void setAsMainScope();
    void run();
};


#endif // PILANG_SCOPE_H
