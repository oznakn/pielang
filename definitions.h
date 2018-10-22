#ifndef PIELANG_DEFINITIONS_H
#define PIELANG_DEFINITIONS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

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

typedef std::function<Value*(ValueList*)> FunctionCallback;

#endif //PIELANG_DEFINITIONS_H
