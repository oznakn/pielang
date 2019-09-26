#include "system.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "value.h"
#include "scope.h"
#include "utils.h"


#define BUFFER_SIZE 100000


Value *system_function_print(Value *context_value, TupleValue *parameter_values) {
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


Value *system_function_min(Value *context_value, TupleValue *parameter_values) {
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


Value *system_function_max(Value *context_value, TupleValue *parameter_values) {
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


Value *system_function_input(Value *context_value, TupleValue *parameter_values) {
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


Value *system_function_number(Value *context_value, TupleValue *parameter_values) {
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


Value *system_function_len(Value *context_value, TupleValue *parameter_values) {
  if (parameter_values->length == 0) return new_null_value();

  Value *value = parameter_values->items[0];

  if (value->value_type == ValueTypeStringValue) {
    StringValue *string_value = (StringValue *) value;

    return new_integer_value(string_value->length);
  }
  else if (value->value_type == ValueTypeTupleValue) {
    TupleValue *tuple_value = (TupleValue *) value;

    return new_integer_value(tuple_value->length);
  }
  else if (value->value_type == ValueTypeListValue) {
    ListValue *list_value = (ListValue *) value;

    return new_integer_value(list_value->length);
  }

  return new_null_value();
}


Value *system_function_list_push(Value *context_value, TupleValue *parameter_values) {
  if (parameter_values->length == 0) return new_null_value();

  ListValue *list_value = (ListValue *) context_value;

  list_value->items = realloc(list_value->items, (list_value->length + parameter_values->length) * sizeof(Value *));

  for (size_t i = 0; i < parameter_values->length; i++) {
    list_value->items[list_value->length + i] = copy_value(parameter_values->items[i]);
  }

  list_value->length += parameter_values->length;

  return new_null_value();
}

Value *system_function_list_pop(Value *context_value, TupleValue *parameter_values) {
  Value *result_value = new_null_value();
  ListValue *list_value = (ListValue *) context_value;
  long long int mid;

  if (parameter_values->length == 0) {
    mid = -1;
  }
  else {
    if (parameter_values->items[0]->value_type != ValueTypeIntegerValue) return result_value;
    mid = normalize_index(((IntegerValue *) parameter_values->items[0])->integer_value, list_value->length);
  }

  if (list_value->length <= mid) return result_value;

  result_value = list_value->items[mid];

  for (size_t i = mid; i < list_value->length - 1; i++) {
    list_value->items[i] = list_value->items[i + 1];
  }

  list_value->items = realloc(list_value->items, (list_value->length - 1) * sizeof(Value *));
  list_value->length -= 1;

  return result_value;
}

void build_system_function(Scope *scope, char *name, Value *value) {
  SystemFunctionValue *system_function_value = (SystemFunctionValue *) value;

  Variable *variable = scope_set_variable(scope, system_function_value->context_value_type, name, value, true);

  variable->is_readonly = true;
}


void build_main_scope(Scope *scope) {
  build_system_function(scope, "print", new_system_function_value(ValueTypeNullValue, system_function_print));
  build_system_function(scope, "min", new_system_function_value(ValueTypeNullValue, system_function_min));
  build_system_function(scope, "max", new_system_function_value(ValueTypeNullValue, system_function_max));
  build_system_function(scope, "input", new_system_function_value(ValueTypeNullValue, system_function_input));
  build_system_function(scope, "number", new_system_function_value(ValueTypeNullValue, system_function_number));
  build_system_function(scope, "len", new_system_function_value(ValueTypeNullValue, system_function_len));
  build_system_function(scope, "push", new_system_function_value(ValueTypeListValue, system_function_list_push));
  build_system_function(scope, "pop", new_system_function_value(ValueTypeListValue, system_function_list_pop));
}

