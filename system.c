#include "system.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "value.h"
#include "scope.h"
#include "utils.h"


#define BUFFER_SIZE 100000


Value *system_function_print(TupleValue *parameter_values) {
  if (parameter_values->length == 0) {
    return new_null_value();
  }

  for (size_t i = 0; i < parameter_values->length; i++) {
    StringValue *string_value = (StringValue *) convert_to_string_value(parameter_values->items[i]);

    printf("%s", string_value->string_value);

    if (i != parameter_values->length - 1) {
      printf(" ");
    }

    free_value((Value *) string_value);
  }

  printf("\n");

  return new_null_value();
}


Value *system_function_min(TupleValue *parameter_values) {
  if (parameter_values->length == 0) {
    return new_null_value();
  }

  long double val;

  if (parameter_values->items[0]->value_type == ValueTypeIntegerValue) {
    val = ((IntegerValue *) parameter_values->items[0])->integer_value;
  }
  else if (parameter_values->items[0]->value_type == ValueTypeFloatValue) {
    val = ((FloatValue *) parameter_values->items[0])->float_value;
  }
  else {
    val = convert_to_integer(parameter_values->items[0]);
  }

  for (size_t i = 0; i < parameter_values->length; i++) {
    long double temp;

    if (parameter_values->items[i]->value_type == ValueTypeIntegerValue) {
      temp = ((IntegerValue *) parameter_values->items[i])->integer_value;
    }
    else if (parameter_values->items[i]->value_type == ValueTypeFloatValue) {
      temp = ((FloatValue *) parameter_values->items[i])->float_value;
    }
    else {
      temp = convert_to_integer(parameter_values->items[i]);
    }

    if (temp < val) {
      val = temp;
    }
  }

  if (ceil(val) == floor(val)) {
    return new_integer_value((long long int) val);
  }
  else {
    return new_float_value(val);
  }
}


Value *system_function_max(TupleValue *parameter_values) {
  if (parameter_values->length == 0) {
    return new_null_value();
  }

  long double val;

  if (parameter_values->items[0]->value_type == ValueTypeIntegerValue) {
    val = ((IntegerValue *) parameter_values->items[0])->integer_value;
  }
  else if (parameter_values->items[0]->value_type == ValueTypeFloatValue) {
    val = ((FloatValue *) parameter_values->items[0])->float_value;
  }
  else {
    val = convert_to_integer(parameter_values->items[0]);
  }

  for (size_t i = 0; i < parameter_values->length; i++) {
    long double temp;

    if (parameter_values->items[i]->value_type == ValueTypeIntegerValue) {
      temp = ((IntegerValue *) parameter_values->items[i])->integer_value;
    }
    else if (parameter_values->items[i]->value_type == ValueTypeFloatValue) {
      temp = ((FloatValue *) parameter_values->items[i])->float_value;
    }
    else {
      temp = convert_to_integer(parameter_values->items[i]);
    }

    if (temp > val) {
      val = temp;
    }
  }

  if (ceil(val) == floor(val)) {
    return new_integer_value((long long int) val);
  }
  else {
    return new_float_value(val);
  }
}


Value *system_function_input(TupleValue *parameter_values) {
  if (parameter_values->length > 0) {
    StringValue *string_value = (StringValue *) convert_to_string_value(parameter_values->items[0]);

    printf("%s", string_value->string_value);

    free_value((Value *) string_value);
  }

  char buffer[BUFFER_SIZE];
  size_t buffer_length = 0;

  char c;

  while (true) {
    c = getchar_unlocked();

    if (c == EOF || c == '\n') break;
    else if (c < ' ') continue;

    buffer[buffer_length++] = c;
  }

  return new_string_value(create_string_from_buffer(buffer, buffer_length), buffer_length);
}


Value *system_function_number(TupleValue *parameter_values) {
  if (parameter_values->length == 0) return new_null_value();

  Value *value = parameter_values->items[0];

  if (value->value_type == ValueTypeIntegerValue || value->value_type == ValueTypeFloatValue) {
    return copy_value(value);
  }
  else if (value->value_type == ValueTypeStringValue) {
    long double float_value = strtold(((StringValue *) value)->string_value, NULL);

    if (ceil(float_value) == floor(float_value)) {
      return new_integer_value((long long int) float_value);
    }
    else {
      return new_float_value(float_value);
    }
  }

  return new_integer_value(convert_to_integer(value));
}


void build_system_function(Scope *scope, char *name, Value *value) {
  Variable *variable = scope_set_variable(scope, name, value, true);

  variable->is_readonly = true;
}


void build_main_scope(Scope *scope) {
  build_system_function(scope, "print", new_system_function_value(system_function_print));
  build_system_function(scope, "min", new_system_function_value(system_function_min));
  build_system_function(scope, "max", new_system_function_value(system_function_max));
  build_system_function(scope, "input", new_system_function_value(system_function_input));
  build_system_function(scope, "number", new_system_function_value(system_function_number));
}

