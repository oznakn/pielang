#ifndef PILANG_DEFINITIONS_H
#define PILANG_DEFINITIONS_H

#include <string>
#include <vector>
#include <unordered_map>

class Value;
class Function;
class Variable;
class Expression;
class Object;
class Operation;
class Scope;
class StringLiteral;
class SystemFunction;

typedef std::vector<std::string> StringList;
typedef std::vector<Value*> ValueList;
typedef std::vector<Function*> FunctionList;
typedef std::vector<Variable*> VariableList;
typedef std::vector<Object*> ObjectList;

typedef std::unordered_map<std::string, Variable*> VariableMap;
typedef std::unordered_map<std::string, Function*> FunctionMap;
typedef std::unordered_map<std::string, Object*> ObjectMap;

typedef Value* (FunctionCallback)(ValueList*);

#endif //PILANG_DEFINITIONS_H
