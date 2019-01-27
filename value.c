#include "value.h"

#include <stdlib.h>

#include "bool.h"
#include "lexer.h"


Value *new_bool_value(bool val) {
  BoolValue *bool_value = malloc(sizeof(BoolValue));

  bool_value->value = (Value){.value_type = ValueTypeBoolValue, .linked_variable_count = 0};
  bool_value->bool_value = val;

  return (Value *)bool_value;
}


Value *new_bool_value_from_literal(BoolLiteral *literal) {
  return new_bool_value(literal->bool_literal);
}


Value *new_integer_value(long int val) {
  IntegerValue *integer_value = malloc(sizeof(IntegerValue));

  integer_value->value = (Value){.value_type = ValueTypeIntegerValue, .linked_variable_count = 0};
  integer_value->integer_value = val;

  return (Value *)integer_value;
}


Value *new_integer_value_from_literal(IntegerLiteral *literal) {
  return new_integer_value(literal->integer_literal);
}


Value *new_float_value(double val) {
  FloatValue *float_value = malloc(sizeof(FloatValue));

  float_value->value = (Value){.value_type = ValueTypeFloatValue, .linked_variable_count = 0};
  float_value->float_value = val;

  return (Value *)float_value;
}


Value *new_float_value_from_literal(FloatLiteral *literal) {
  return new_float_value(literal->float_literal);
}


Value *new_string_value(char *val, size_t length) {
  StringValue *string_value = malloc(sizeof(StringValue));

  string_value->value = (Value){.value_type = ValueTypeStringValue};
  string_value->string_value = val;
  string_value->length = length;

  return (Value *)string_value;
}


Value *new_string_value_from_literal(StringLiteral *literal) {
  return new_string_value(literal->string_literal, literal->length);
}


Value *new_function_value(Block *block, char *function_name, char **arguments, size_t argument_count) {
  FunctionValue *function_value = malloc(sizeof(FunctionValue));

  function_value->value = (Value){.value_type = ValueTypeFunctionValue, .linked_variable_count = 0};
  function_value->block = block;
  function_value->function_name = function_name;
  function_value->arguments = arguments;
  function_value->argument_count = argument_count;

  return (Value *)function_value;
}


Value *new_tuple_value(Value **items, size_t length) {
  TupleValue *tuple_value = malloc(sizeof(TupleValue));

  tuple_value->value = (Value){.value_type =ValueTypeTupleValue, .linked_variable_count = 0};
  tuple_value->items = items;
  tuple_value->length = length;

  return (Value *)tuple_value;
}


Value *new_list_value(Value **items, size_t length) {
  ListValue *list_value = malloc(sizeof(ListValue));

  list_value->value = (Value){.value_type =ValueTypeListValue, .linked_variable_count = 0};
  list_value->items = items;
  list_value->length = length;

  return (Value *)list_value;
}


Variable *new_variable(char *variable_name, Value *value) {
  Variable *variable = malloc(sizeof(Variable));

  variable->variable_name = variable_name;
  variable->value = value;
  variable->value->linked_variable_count++;

  switch (variable->value->value_type) {
    case ValueTypeTupleValue: {
      TupleValue *tuple_value = (TupleValue *)variable->value;

      for (size_t i = 0; i < tuple_value->length; i += 1u) {
        tuple_value->items[i]->linked_variable_count++;
      }
    }

    case ValueTypeListValue: {
      ListValue *list_value = (ListValue *)variable->value;

      for (size_t i = 0; i < list_value->length; i += 1u) {
        list_value->items[i]->linked_variable_count++;
      }
    }

    default : {

    }
  }

  return variable;
}


HashTable *new_variable_map(size_t size) {
  return new_hash_table(size, HashTableTypeVariableMap);
}


void variable_map_set(HashTable *variable_map, Variable *variable) {
  return hash_table_set(variable_map, variable->variable_name, variable);
}


void *variable_map_get(HashTable *variable_map, char *variable_name) {
  return hash_table_get(variable_map, variable_name);
}


void free_variable_map(HashTable *variable_map) {
  free_hash_table(variable_map);
}


void safe_free_value(Value *value) {
  value->linked_variable_count--;

  if (value->linked_variable_count == 0) {
    switch (value->value_type) {
      case ValueTypeTupleValue: {
        TupleValue *tuple_value = (TupleValue *)value;

        for (size_t i = 0; i < tuple_value->length; i += 1u) {
          safe_free_value(tuple_value->items[i]);
        }
      }

      case ValueTypeListValue: {
        ListValue *list_value = (ListValue *)value;

        for (size_t i = 0; i < list_value->length; i += 1u) {
          safe_free_value(list_value->items[i]);
        }
      }

      default : {

      }
    }

    free_value(value);
  }
}


void free_value(Value *value) {
  switch (value->value_type) {
    case ValueTypeBoolValue: {
      BoolValue *bool_value = (BoolValue *)value;
      free(bool_value);
      break;
    }

    case ValueTypeIntegerValue: {
      IntegerValue *integer_value = (IntegerValue *)value;
      free(integer_value);
      break;

    }

    case ValueTypeFloatValue: {
      FloatValue *float_value = (FloatValue *)value;
      free(float_value);
      break;
    }

    case ValueTypeStringValue: {
      StringValue *string_value = (StringValue *)value;
      free(string_value);
      break;
    }

    case ValueTypeFunctionValue: {
      FunctionValue *function_value = (FunctionValue *)value;

      for (size_t i = 0; i < function_value->argument_count; i += 1u) {
        free(function_value->arguments[i]);
      }

      free_block(function_value->block);
      free(function_value);
      break;
    }

    case ValueTypeTupleValue: {
      TupleValue *tuple_value = (TupleValue *)value;

      for (size_t i = 0; i < tuple_value->length; i += 1u) {
        free_value(tuple_value->items[i]); // TODO
      }

      free(tuple_value);
      break;
    }

    case ValueTypeListValue: {
      ListValue *list_value = (ListValue *)value;

      for (size_t i = 0; i < list_value->length; i += 1u) {
        free_value(list_value->items[i]); // TODO
      }

      free(list_value);
      break;
    }

    case ValueTypeObjectValue: {
      ObjectValue *object_value = (ObjectValue *)value;

      free_hash_table(object_value->variable_map);
      free(object_value);
      break;
    }

    default: {

    }
  }
}


void free_variable(Variable *variable) {
  safe_free_value(variable->value);

  free(variable);
}
