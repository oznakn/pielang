#ifndef PIELANG_VALUE_H
#define PIELANG_VALUE_H

#include "bool.h"
#include "lexer.h"
#include "ast.h"
#include "hashtable.h"
#include "scope.h"

typedef enum {
  ValueTypeNullValue = 0,
  ValueTypeBoolValue,
  ValueTypeIntegerValue,
  ValueTypeFloatValue,
  ValueTypeStringValue,
  ValueTypeFunctionValue,
  ValueTypeSystemFunctionValue,
  ValueTypeTupleValue,
  ValueTypeListValue,
  ValueTypeGeneratorValue,
} ValueType;

typedef enum {
  GeneratorValueTypeNumber = 1,
  GeneratorValueTypeArray,
} GeneratorValueType;

struct Value {
  ValueType value_type;
  size_t linked_variable_count;
};

struct BoolValue {
  struct Value value;
  bool bool_value;
};

struct IntegerValue {
  struct Value value;
  long long int integer_value;
};

struct FloatValue {
  struct Value value;
  long double float_value;
};

struct StringValue {
  struct Value value;
  char *string_value;
  size_t length;
};

struct TupleValue {
  struct Value value;
  struct Value **items;
  size_t length;
  bool has_finished;
};

struct ListValue {
  struct Value value;
  struct Value **items;
  size_t length;
  bool has_finished;
};

struct FunctionValue {
  struct Value value;
  Block *block;
  char **arguments;
  size_t argument_count;
};

typedef struct Value *(SystemFunctionCallback)(struct TupleValue *);

struct SystemFunctionValue {
  struct Value value;
  SystemFunctionCallback *callback;
};

struct GeneratorValue {
  struct Value value;
  GeneratorValueType generator_value_type;
  struct Value **target_values;
  long long int start_value;
  long long int end_value;
  long long int index;
};

struct Variable {
  char *variable_name;
  struct Value *value;
  bool is_readonly;
};

typedef struct Value Value;
typedef struct BoolValue BoolValue;
typedef struct IntegerValue IntegerValue;
typedef struct FloatValue FloatValue;
typedef struct StringValue StringValue;
typedef struct TupleValue TupleValue;
typedef struct ListValue ListValue;
typedef struct FunctionValue FunctionValue;
typedef struct SystemFunctionValue SystemFunctionValue;
typedef struct GeneratorValue GeneratorValue;
typedef struct Variable Variable;


char *convert_to_string(Value *value);


bool convert_to_bool(Value *value);


long long int convert_to_integer(Value *value);




Value *new_null_value();


Value *new_bool_value(bool val);


Value *new_bool_value_from_literal(BoolLiteral *literal);


Value *new_integer_value(long long int val);


Value *new_integer_value_from_literal(IntegerLiteral *literal);


Value *new_float_value(long double val);


Value *new_float_value_from_literal(FloatLiteral *literal);


Value *new_string_value(char *val, size_t length);


Value *new_string_value_from_literal(StringLiteral *literal);


Value *new_function_value(Block *block, char **arguments, size_t argument_count);


Value *new_system_function_value(SystemFunctionCallback *callback);


Value *new_tuple_value(Value **items, size_t length, bool has_finished);


Value *new_list_value(Value **items, size_t length, bool has_finished);


Value *new_generator_value(GeneratorValueType generator_value_type, Value *first_value, Value *second_value);


Value *fetch_value_from_generator_value(GeneratorValue *generator_value);



Value *convert_to_generator_value(Value *value);


Value *convert_to_string_value(Value *value);


Value *convert_to_bool_value(Value *value);


Value *convert_to_integer_value(Value *value);


Value *copy_value(Value *value);




Variable *new_variable(char *variable_name, Value *value);


HashTable *new_variable_map(size_t size);


void variable_map_set(HashTable *variable_map, Variable *variable);


Variable *variable_map_get(HashTable *variable_map, char *variable_name);


void free_variable_map(HashTable *variable_map);


void free_value(Value *value);


void free_variable(Variable *variable);


#endif //PIELANG_VALUE_H
