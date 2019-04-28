#include "system.h"

#include <string.h>
#include <stdlib.h>
#include "value.h"

Value *apply_prefix_plus_operation(Value *right_value) {
  if (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue) {
    return copy_value(right_value);
  }

  return new_null_value();
}


Value *apply_prefix_minus_operation(Value *right_value) {
  if (right_value->value_type == ValueTypeIntegerValue) {
    return new_integer_value(-((IntegerValue *) right_value)->integer_value);
  }
  else if (right_value->value_type == ValueTypeFloatValue) {
    return new_float_value(-((FloatValue *) right_value)->float_value);
  }

  return new_null_value();
}


Value *apply_addition_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeStringValue || right_value->value_type == ValueTypeStringValue) {
    StringValue *left_string_value = (StringValue *) convert_to_string_value(left_value);
    StringValue *right_string_value = (StringValue *) convert_to_string_value(right_value);

    size_t length = left_string_value->length + right_string_value->length;

    char *result_string = calloc(length + 1, sizeof(char));

    strcpy(result_string, left_string_value->string_value);
    strcat(result_string, right_string_value->string_value);

    free_value((Value *) left_string_value);
    free_value((Value *) right_string_value);

    return new_string_value(result_string, length);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
      return new_integer_value(((IntegerValue *) left_value)->integer_value + ((IntegerValue *) right_value)->integer_value);
    }
    else {
      double left_float, right_float;

      if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
      else left_float = ((IntegerValue *) left_value)->integer_value;

      if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
      else right_float = ((IntegerValue *) right_value)->integer_value;

      return new_float_value(left_float + right_float);
    }
  }
  else if ((left_value->value_type == ValueTypeTupleValue || left_value->value_type == ValueTypeListValue) && (right_value->value_type == ValueTypeTupleValue || right_value->value_type == ValueTypeListValue)) {
    Value **left_array_items, **right_array_items, **result_items;
    size_t left_array_item_length, right_array_item_length;

    if (left_value->value_type == ValueTypeListValue) {
      ListValue *list_value = (ListValue *) left_value;

      left_array_items = list_value->items;
      left_array_item_length = list_value->length;
    }
    else {
      TupleValue *tuple_value = (TupleValue *) left_value;

      left_array_items = tuple_value->items;
      left_array_item_length = tuple_value->length;
    }

    if (right_value->value_type == ValueTypeListValue) {
      ListValue *list_value = (ListValue *) right_value;

      right_array_items = list_value->items;
      right_array_item_length = list_value->length;
    }
    else {
      TupleValue *tuple_value = (TupleValue *) right_value;

      right_array_items = tuple_value->items;
      right_array_item_length = tuple_value->length;
    }

    result_items = malloc((left_array_item_length + right_array_item_length) * sizeof(Value *));

    memcpy(result_items, left_array_items, left_array_item_length * sizeof(Value *));
    memcpy(&result_items[left_array_item_length], right_array_items, right_array_item_length * sizeof(Value *));

    return new_tuple_value(result_items, left_array_item_length + right_array_item_length);
  }

  return new_null_value();
}


Value *apply_subtraction_operation(Value *left_value, Value *right_value) {
  if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
      return new_integer_value(((IntegerValue *) left_value)->integer_value - ((IntegerValue *) right_value)->integer_value);
    }
    else {
      double left_float, right_float;

      if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
      else left_float = ((IntegerValue *) left_value)->integer_value;

      if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
      else right_float = ((IntegerValue *) right_value)->integer_value;

      return new_float_value(left_float - right_float);
    }
  }

  return new_null_value();
}


Value *apply_multiplication_operation(Value *left_value, Value *right_value) {
  return new_null_value();
}


Value *apply_division_operation(Value *left_value, Value *right_value) {
  return new_null_value();
}


Value *apply_exponent_operation(Value *left_value, Value *right_value) {
  return new_null_value();
}


Value *apply_mod_operation(Value *left_value, Value *right_value) {
  return new_null_value();
}

