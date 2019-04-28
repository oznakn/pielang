#ifndef PIELANG_VALUE_H
#define PIELANG_VALUE_H

#include "bool.h"
#include "lexer.h"
#include "ast.h"
#include "hashtable.h"

struct Variable;

typedef struct {
  char *class_name;
  HashTable *variable_map;
} Class;


typedef enum {
  ValueTypeNullValue = 0,
  ValueTypeBoolValue,
  ValueTypeIntegerValue,
  ValueTypeFloatValue,
  ValueTypeStringValue,
  ValueTypeFunctionValue,
  ValueTypeObjectValue,
  ValueTypeTupleValue,
  ValueTypeListValue,
} ValueType;


typedef struct {
  ValueType value_type;
  size_t linked_variable_count;
} Value;


typedef struct {
  Value value;
  bool bool_value;
} BoolValue;


typedef struct {
  Value value;
  long int integer_value;
} IntegerValue;


typedef struct {
  Value value;
  double float_value;
} FloatValue;


typedef struct {
  Value value;
  char *string_value;
  size_t length;
} StringValue;


typedef struct {
  Value value;
  Block *block;
  char *function_name;
  char **arguments;
  size_t argument_count;
} FunctionValue;


typedef struct {
  Value value;
  Value **items;
  size_t length;
  bool has_finished;
} TupleValue, ListValue;


typedef struct {
  Value value;
  Class *class;
  HashTable *variable_map;
} ObjectValue;

struct Variable {
  char *variable_name;
  Value *value;
  bool is_readonly;
};
typedef struct Variable Variable;


Value *new_null_value();


Value *new_bool_value(bool val);


Value *new_bool_value_from_literal(BoolLiteral *literal);


Value *new_integer_value(long int val);


Value *new_integer_value_from_literal(IntegerLiteral *literal);


Value *new_float_value(double val);


Value *new_float_value_from_literal(FloatLiteral *literal);


Value *new_string_value(char *val, size_t length);


Value *new_string_value_from_literal(StringLiteral *literal);


Value *convert_to_string_value(Value *value);


Value *new_function_value(Block *block, char *function_name, char **arguments, size_t argument_count);


Value *new_object_value();


Value *new_tuple_value(Value **items, size_t length, bool has_finished);


Value *new_list_value(Value **items, size_t length, bool has_finished);


Value *copy_value(Value *value);


Variable *new_variable(char *variable_name, Value *value);


HashTable *new_variable_map(size_t size);


void variable_map_set(HashTable *variable_map, Variable *variable);


Variable *variable_map_get(HashTable *variable_map, char *variable_name);


void free_variable_map(HashTable *variable_map);


void free_value(Value *value);


void free_variable(Variable *variable);


#endif //PIELANG_VALUE_H
