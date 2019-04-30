#include "value.h"

#include <stdlib.h>

#include "bool.h"
#include "lexer.h"

#define MAX_BUFFER_SIZE 10000

#include "stringutils.h"


Value *new_null_value() {
  static Value *value;

  if (value == NULL) {
    value = malloc(sizeof(Value));

    value->value_type = ValueTypeNullValue;
    value->linked_variable_count = 0; // no need for others since they are not taken with malloc, the default value for an int in a struct is 0
  }

  return value;
}


Value *new_bool_value(bool val) {
  BoolValue *bool_value = malloc(sizeof(BoolValue));

  bool_value->value = (Value){.value_type = ValueTypeBoolValue};
  bool_value->bool_value = val;

  return (Value *) bool_value;
}


Value *new_bool_value_from_literal(BoolLiteral *literal) {
  return new_bool_value(literal->bool_literal);
}


Value *new_integer_value(long long int val) {
  IntegerValue *integer_value = malloc(sizeof(IntegerValue));

  integer_value->value = (Value){.value_type = ValueTypeIntegerValue};
  integer_value->integer_value = val;

  return (Value *)integer_value;
}


Value *new_integer_value_from_literal(IntegerLiteral *literal) {
  return new_integer_value(literal->integer_literal);
}


Value *new_float_value(long double val) {
  FloatValue *float_value = malloc(sizeof(FloatValue));

  float_value->value = (Value){.value_type = ValueTypeFloatValue};
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
  char *s = calloc(literal->length + 1, sizeof(char));

  strcpy(s, literal->string_literal);

  return new_string_value(s, literal->length);
}


Value *convert_to_string_value(Value *value) {
  switch (value->value_type) {
    case ValueTypeNullValue: {
      char *s = calloc(5, sizeof(char));
      strcpy(s, "null");
      return new_string_value(s, 5);
    }

    case ValueTypeBoolValue: {
      BoolValue *bool_value = (BoolValue *) value;

      if (bool_value->bool_value) {
        char *s = calloc(5, sizeof(char));

        strcpy(s, "true");

        return new_string_value(s, 5);
      }
      else {
        char *s = calloc(6, sizeof(char));

        strcpy(s, "false");

        return new_string_value(s, 6);
      }
    }

    case ValueTypeIntegerValue: {
      char buffer[MAX_BUFFER_SIZE] = {};

      IntegerValue *integer_value = (IntegerValue *) value;

      sprintf(buffer, "%lld", integer_value->integer_value);

      int length = strlen(buffer);
      char *s = calloc(length + 1, sizeof(length));

      strcpy(s, buffer);

      return new_string_value(s, length);
    }

    case ValueTypeFloatValue: {
      char buffer[MAX_BUFFER_SIZE] = {};

      FloatValue *float_value = (FloatValue *) value;

      sprintf(buffer, "%.2Lf", float_value->float_value);

      size_t length = strlen(buffer);

      return new_string_value(create_string_from_buffer(buffer, length), length);
    }

    case ValueTypeStringValue: {
      return copy_value(value);
    }

    case ValueTypeTupleValue: {
      char buffer[MAX_BUFFER_SIZE] = {};

      TupleValue *tuple_value = (TupleValue *) value;

      strcpy(buffer, "T(");

      for (size_t i = 0; i < tuple_value->length; i++) {
        StringValue *string_value = (StringValue *) convert_to_string_value(tuple_value->items[i]);

        if (tuple_value->items[i]->value_type == ValueTypeStringValue) {
          strcat(buffer, "\'");
          strcat(buffer, string_value->string_value);
          strcat(buffer, "\'");
        }
        else {
          strcat(buffer, string_value->string_value);
        }

        free_value((Value *) string_value);

        if (i != tuple_value->length - 1) {
          strcat(buffer, ", ");
        }
      }

      strcat(buffer, ")");

      size_t length = strlen(buffer);

      return new_string_value(create_string_from_buffer(buffer, length), length);
    }

    case ValueTypeListValue: {
      char buffer[MAX_BUFFER_SIZE] = {};

      ListValue *list_value = (ListValue *) value;

      strcpy(buffer, "[");

      for (size_t i = 0; i < list_value->length; i++) {
        StringValue *string_value = (StringValue *) convert_to_string_value(list_value->items[i]);

        strcat(buffer, string_value->string_value);

        free_value((Value *) string_value);

        if (i != list_value->length - 1) {
          strcat(buffer, ", ");
        }
      }

      strcat(buffer, "]");

      size_t length = strlen(buffer);

      return new_string_value(create_string_from_buffer(buffer, length), length);
    }

    default: {
      return NULL;
    }
  }
}


Value *convert_to_bool_value(Value *value) {
  switch (value->value_type) {
    case ValueTypeNullValue: {
      return new_bool_value(false);
    }

    case ValueTypeBoolValue: {
      return copy_value(value);
    }

    case ValueTypeIntegerValue: {
      return new_bool_value(((IntegerValue *) value)->integer_value != 0);
    }

    case ValueTypeFloatValue: {
      return new_bool_value(((FloatValue *) value)->float_value != 0);
    }

    case ValueTypeStringValue: {
      StringValue *string_value = (StringValue *) value;

      return new_bool_value(string_value->length != 0);
    }

    case ValueTypeTupleValue: {
      return new_bool_value(((TupleValue *) value)->length != 0);
    }

    case ValueTypeListValue: {
      return new_bool_value(((ListValue *) value)->length != 0);
    }

    default: {
      return NULL;
    }
  }
}

Value *new_function_value(Block *block, char **arguments, size_t argument_count) {
  FunctionValue *function_value = malloc(sizeof(FunctionValue));

  function_value->value = (Value){.value_type = ValueTypeFunctionValue};
  function_value->block = block;
  function_value->arguments = arguments;
  function_value->argument_count = argument_count;

  return (Value *)function_value;
}


Value *new_system_function_value(SystemFunctionCallback *callback) {
  SystemFunctionValue *system_function_value = malloc(sizeof(SystemFunctionValue));

  system_function_value->value = (Value) {.value_type = ValueTypeSystemFunctionValue};
  system_function_value->callback = callback;

  return (Value *) system_function_value;
}


Value *new_tuple_value(Value **items, size_t length, bool has_finished) {
  TupleValue *tuple_value = malloc(sizeof(TupleValue));

  tuple_value->value = (Value){.value_type = ValueTypeTupleValue};
  tuple_value->items = items;
  tuple_value->length = length;
  tuple_value->has_finished = has_finished;

  return (Value *)tuple_value;
}


Value *new_list_value(Value **items, size_t length, bool has_finished) {
  ListValue *list_value = malloc(sizeof(ListValue));

  list_value->value = (Value){.value_type =ValueTypeListValue};
  list_value->items = items;
  list_value->length = length;
  list_value->has_finished = has_finished;

  return (Value *)list_value;
}


Class *new_class(char *class_name, Scope *scope) {
  Class *class = malloc(sizeof(Class));

  class->class_name = class_name;
  class->scope = new_scope(scope, NULL, false);

  return class;
}


Value *new_object_value(Class *class) {
  ObjectValue *object_value = malloc(sizeof(ObjectValue));

  object_value->value = (Value) {.value_type = ValueTypeObjectValue};
  object_value->class = class;
  object_value->scope = new_scope(class->scope, NULL, false);

  return (Value *) object_value;
}


Value *copy_value(Value *value) {
  Value *result_value;

  switch(value->value_type) {
    case ValueTypeBoolValue: {
      result_value = new_bool_value(((BoolValue*) value)->bool_value);
      break;
    }

    case ValueTypeIntegerValue: {
      result_value = new_integer_value(((IntegerValue*) value)->integer_value);
      break;
    }

    case ValueTypeFloatValue: {
      result_value = new_float_value(((FloatValue*) value)->float_value);
      break;
    }

    case ValueTypeStringValue: {
      StringValue *string_value = (StringValue *) value;

      char *s = calloc(string_value->length + 1, sizeof(char));

      strcpy(s, string_value->string_value);

      result_value = new_string_value(s, string_value->length);

      break;
    }

    case ValueTypeTupleValue: {
      TupleValue *tuple_value = (TupleValue *) value;

      Value **items = malloc(tuple_value->length * sizeof(Value *));

      memcpy(items, tuple_value->items, tuple_value->length * sizeof(Value *));

      result_value = new_tuple_value(items, tuple_value->length, tuple_value->has_finished);

      break;
    }

    case ValueTypeListValue: {
      ListValue *list_value = (ListValue *) value;

      Value **items = malloc(list_value->length * sizeof(Value *));

      memcpy(items, list_value->items, list_value->length * sizeof(Value *));

      result_value = new_list_value(items, list_value->length, list_value->has_finished);

      break;
    }

    default: {
      result_value = new_null_value();
      break;
    }
  }

  return result_value;
}


Variable *new_variable(char *variable_name, Value *value) {
  Variable *variable = malloc(sizeof(Variable));

  variable->variable_name = variable_name;
  variable->value = value;
  variable->value->linked_variable_count++;
  variable->is_readonly = false;

  return variable;
}


HashTable *new_variable_map(size_t size) {
  return new_hash_table(size, HashTableTypeVariableMap);
}


void variable_map_set(HashTable *variable_map, Variable *variable) {
  hash_table_set(variable_map, variable->variable_name, variable);
}


Variable *variable_map_get(HashTable *variable_map, char *variable_name) {
  return (Variable *) hash_table_get(variable_map, variable_name);
}


Variable *object_value_set_variable(ObjectValue *object_value, char *name, Value *value) {
  return scope_set_variable(object_value->scope, name, value, 0, true);
}


Variable *object_value_get_variable(ObjectValue *object_value, char *name) {
  return scope_get_variable(object_value->scope, name);
}


void free_variable_map(HashTable *variable_map) {
  free_hash_table(variable_map);
}


void free_value(Value *value) {
  if (value->value_type != ValueTypeNullValue && value->linked_variable_count == 0) {
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
        free(string_value->string_value);
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

      case ValueTypeSystemFunctionValue: {
        SystemFunctionValue *system_function_value = (SystemFunctionValue *)value;

        free(system_function_value);
        break;
      }

      case ValueTypeTupleValue: {
        TupleValue *tuple_value = (TupleValue *) value;

        for (size_t i = 0; i < tuple_value->length; i += 1u) {
          tuple_value->items[i]->linked_variable_count--;
          free_value(tuple_value->items[i]);
        }

        free(tuple_value->items);
        free(tuple_value);
        break;
      }

      case ValueTypeListValue: {
        ListValue *list_value = (ListValue *)value;

        for (size_t i = 0; i < list_value->length; i += 1u) {
          list_value->items[i]->linked_variable_count--;
          free_value(list_value->items[i]);
        }

        free(list_value->items);
        free(list_value);
        break;
      }

      case ValueTypeObjectValue: {
        ObjectValue *object_value = (ObjectValue *)value;

        free_scope(object_value->scope);
        free(object_value);
        break;
      }

      default: {

      }
    }
  }
}


void free_variable(Variable *variable) {
  if (variable->value != NULL) {
    variable->value->linked_variable_count--;

    free_value(variable->value);
  }
  free(variable->variable_name);
  free(variable);
}
