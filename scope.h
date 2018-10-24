#ifndef PIELANG_SCOPE_H
#define PIELANG_SCOPE_H

#include <string>
#include <vector>

#include "definitions.h"

class Scope {
private:
    static void runLine(std::string);

    Scope* mInheritedScope = nullptr;
    std::string mContent;

    Object* mScopeObject;

    Value* mScopeResult;

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

    void setScopeResult(Value*);
    Value* getScopeResult();

    VariableMap* getVariableMap();
    FunctionMap* getFunctionMap();

    Value* parseValue(std::string);
    ValueList* parsePluralValue(std::string);
    void setAsMainScope();
    void run();

    Variable* getVariable(std::string);
    bool hasVariable(std::string);
    void addVariable(std::string, Variable*);
    void removeVariable(std::string);
    Variable* createVariable(std::string, Value*);

    Function* getFunction(std::string);
    bool hasFunction(std::string);
    void addFunction(std::string, Function*);
    void removeFunction(std::string);
    Function* createSystemFunction(std::string, FunctionCallback);

    Object* getObject(std::string);
    bool hasObject(std::string);
    void addObject(std::string, Object*);
    void removeObject(std::string);
    Object* createObject(std::string);
};


#endif // PIELANG_SCOPE_H
