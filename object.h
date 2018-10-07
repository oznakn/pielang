#ifndef PILANG_OBJECT_H
#define PILANG_OBJECT_H

#include <string>

#include "definitions.h"


class Object {
private:
    std::string mObjectName;
    VariableMap* mVariableMap;
    FunctionMap* mFunctionMap;
    ObjectMap* mObjectMap;

    ObjectList* mLinkedObjectList = nullptr;

public:
    Object();
    Object(std::string);
    ~Object();

    Object* getWorkingObject(std::string&);
    std::string getRealNameFromDotNotation(std::string&);

    void linkWithObject(Object* object);
    void unlinkWithObject(Object* object);
    void deleteIfNotLinked();

    VariableMap* getVariableMap();
    FunctionMap* getFunctionMap();

    Variable* getVariable(std::string);
    bool hasVariable(std::string);
    void addVariable(std::string, Variable*);
    void removeVariable(std::string);
    Variable* createVariable(std::string, Value*);

    Function* getFunction(std::string);
    bool hasFunction(std::string);
    void addFunction(std::string, Function*);
    void removeFunction(std::string);
    Function* createSystemFunction(std::string, FunctionCallback*);

    Object* getObject(std::string);
    bool hasObject(std::string);
    void addObject(std::string, Object*);
    void removeObject(std::string);
    Object* createObject(std::string);
};


#endif //PILANG_OBJECT_H
