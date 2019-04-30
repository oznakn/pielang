#include "evaluator.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "bool.h"
#include "ast.h"
#include "scope.h"
#include "value.h"
#include "system.h"


Value *_evaluate_or_apply_index_operation(Scope *scope, IndexExpression *index_expression, Value *assign_value) {
  Value **items;
  size_t items_length;

  Value *index_expression_left_value = evaluate_expression(scope, index_expression->left_expression);
  Value *index_expression_right_value = evaluate_expression(scope, index_expression->right_expression);

  if ((index_expression_left_value->value_type != ValueTypeTupleValue &&
       index_expression_left_value->value_type != ValueTypeListValue) ||
      index_expression_right_value->value_type != ValueTypeIntegerValue) return new_null_value();

  IntegerValue *index_expression_right_integer_value = (IntegerValue *) index_expression_right_value;

  if (index_expression_left_value->value_type == ValueTypeListValue) {
    items = ((ListValue *) index_expression_left_value)->items;
    items_length = ((ListValue *) index_expression_left_value)->length;
  }
  else {
    items = ((TupleValue *) index_expression_left_value)->items;
    items_length = ((TupleValue *) index_expression_left_value)->length;
  }

  size_t index_of_item = index_expression_right_integer_value->integer_value;

  if (index_of_item >= items_length) {
    index_of_item = index_of_item % items_length;
  }
  else if (index_of_item < 0) {
    index_of_item = items_length + (index_of_item % items_length);
  }

  if (assign_value == NULL) {
    free_value(index_expression_left_value);
    free_value(index_expression_right_value);

    return items[index_of_item];
  }

  Value *old_value = items[index_of_item];
  items[index_of_item] = assign_value;

  old_value->linked_variable_count--;
  assign_value->linked_variable_count++;

  free_value(old_value);
  free_value(index_expression_left_value);
  free_value(index_expression_right_value);

  return new_null_value();
}


Value *call_system_function(SystemFunctionValue *system_function_value, TupleValue *parameter_values) {
  return system_function_value->callback(parameter_values);
}


Value *call_function(Scope *scope, FunctionValue *function_value, TupleValue *parameter_values) {
  Scope *function_scope = new_scope(scope, function_value->block, ScopeTypeFunctionScope);

  if (parameter_values->length > function_value->argument_count) return new_null_value();

  Value *variable_value;

  for (size_t i = 0; i < function_value->argument_count; i++) {
    if (parameter_values->length <= i) {
      variable_value = new_null_value();
    }
    else {
      variable_value = parameter_values->items[i];
    }

    scope_set_variable(function_scope, function_value->arguments[i], variable_value, true);
  }

  Value *result = evaluate_scope(function_scope);
  free_scope(function_scope);

  return result;
}


Value *apply_prefix_not_operation(Value *right_value) {
  if (right_value->value_type == ValueTypeBoolValue) {
    return new_bool_value(!((BoolValue *) right_value)->bool_value);
  }
  else if (right_value->value_type == ValueTypeIntegerValue) {
    return new_integer_value(!((IntegerValue *) right_value)->integer_value);
  }

  return new_bool_value(!convert_to_bool(right_value));
}


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


Value *apply_index_operation(Scope *scope, IndexExpression *index_expression, Value *assign_value) {
  return _evaluate_or_apply_index_operation(scope, index_expression, assign_value);
}


Value *apply_addition_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeStringValue || right_value->value_type == ValueTypeStringValue) {
    char *s1 = convert_to_string(left_value);
    char *s2 = convert_to_string(right_value);

    size_t length = strlen(s1) + strlen(s2);

    char *result_string = calloc(length + 1, sizeof(char));

    strcpy(result_string, s1);
    strcat(result_string, s2);

    free(s1);
    free(s2);

    return new_string_value(result_string, length);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
      return new_integer_value(((IntegerValue *) left_value)->integer_value + ((IntegerValue *) right_value)->integer_value);
    }
    else {
      long double left_float, right_float;

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

    return new_tuple_value(result_items, left_array_item_length + right_array_item_length, true);
  }

  return new_null_value();
}


Value *apply_subtraction_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_integer_value(((IntegerValue *) left_value)->integer_value - ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_float_value(left_float - right_float);
  }

  return new_null_value();
}


Value *apply_multiplication_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_integer_value(((IntegerValue *) left_value)->integer_value * ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_float_value(left_float * right_float);
  }

  return new_null_value();
}


Value *apply_division_operation(Value *left_value, Value *right_value, bool is_integer_division) {
  if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float, result;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    result = left_float / right_float;

    if (is_integer_division || ceil(result) == floor(result)) {
      return new_integer_value((long long int) result);
    }
    else {
      return new_float_value(result);
    }
  }

  return new_null_value();
}


Value *apply_exponent_operation(Value *left_value, Value *right_value) {
  if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float, result;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    result = pow(left_float, right_float);

    if (ceil(result) == floor(result)) {
      return new_integer_value((long long int) result);
    }
    else {
      return new_float_value(result);
    }
  }

  return new_null_value();
}


Value *apply_mod_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_integer_value(((IntegerValue *) left_value)->integer_value % ((IntegerValue *) right_value)->integer_value);
  }

  return new_null_value();
}


Value *apply_assign_operation(Value *left_value, Value *right_value, Operator operator) {
  Value *result_value = NULL;

  switch (operator) {
    case ASSIGN_OP: {
      result_value = right_value;
      break;
    }

    case ASSIGN_ADDITION_OP:{
      result_value = apply_addition_operation(left_value, right_value);

      break;
    }

    case ASSIGN_SUBTRACTION_OP:{
      result_value = apply_subtraction_operation(left_value, right_value);
      break;
    }

    case ASSIGN_MULTIPLICATION_OP:{
      result_value = apply_multiplication_operation(left_value, right_value);
      break;
    }

    case ASSIGN_DIVISION_OP:{
      result_value = apply_division_operation(left_value, right_value, false);
      break;
    }

    case ASSIGN_INTEGER_DIVISION_OP:{
      result_value = apply_division_operation(left_value, right_value, true);
      break;
    }

    case ASSIGN_EXPONENT_OP:{
      result_value = apply_exponent_operation(left_value, right_value);
      break;
    }

    case ASSIGN_MOD_OP:{
      result_value = apply_mod_operation(left_value, right_value);
      break;
    }

    default: {
      result_value = new_null_value();
      break;
    }
  }

  return result_value;
}


Value *apply_check_equality_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_bool_value(((IntegerValue *) left_value)->integer_value == ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_bool_value(left_float == right_float);
  }
  else if (left_value->value_type == ValueTypeStringValue  && right_value->value_type == ValueTypeStringValue) {
    return new_bool_value(strcmp(((StringValue *) left_value)->string_value, ((StringValue *) left_value)->string_value) == 0);
  }

  return new_bool_value(convert_to_integer(left_value) == convert_to_integer(right_value));
}


Value *apply_check_not_equality_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_bool_value(((IntegerValue *) left_value)->integer_value != ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_bool_value(left_float != right_float);
  }
  else if (left_value->value_type == ValueTypeStringValue  && right_value->value_type == ValueTypeStringValue) {
    return new_bool_value(strcmp(((StringValue *) left_value)->string_value, ((StringValue *) left_value)->string_value) == 0);
  }

  return new_bool_value(convert_to_integer(left_value) != convert_to_integer(right_value));
}


Value *apply_check_bigger_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_bool_value(((IntegerValue *) left_value)->integer_value > ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_bool_value(left_float > right_float);
  }

  return new_bool_value(convert_to_integer(left_value) > convert_to_integer(right_value));
}


Value *apply_check_bigger_equal_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_bool_value(((IntegerValue *) left_value)->integer_value >= ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_bool_value(left_float >= right_float);
  }

  return new_bool_value(convert_to_integer(left_value) >= convert_to_integer(right_value));
}


Value *apply_check_smaller_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_bool_value(((IntegerValue *) left_value)->integer_value < ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_bool_value(left_float < right_float);
  }

  return new_bool_value(convert_to_integer(left_value) < convert_to_integer(right_value));
}


Value *apply_check_smaller_equal_operation(Value *left_value, Value *right_value) {
  if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
    return new_bool_value(((IntegerValue *) left_value)->integer_value <= ((IntegerValue *) right_value)->integer_value);
  }
  else if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    long double left_float, right_float;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    return new_bool_value(left_float <= right_float);
  }

  return new_bool_value(convert_to_integer(left_value) <= convert_to_integer(right_value));
}


Value *evaluate_call_expression(Scope *scope, CallExpression *call_expression) {
  Value *identifier_value = evaluate_expression(scope, call_expression->identifier_expression);
  Value *parameter_values = evaluate_expression(scope, call_expression->tuple_expression);

  Value *result_value = new_null_value();

  if (parameter_values->value_type == ValueTypeTupleValue) {
    if (identifier_value->value_type == ValueTypeFunctionValue) {
      result_value = call_function(scope, (FunctionValue *) identifier_value, (TupleValue *) parameter_values);
    }
    if (identifier_value->value_type == ValueTypeSystemFunctionValue) {
      result_value = call_system_function((SystemFunctionValue *) identifier_value, (TupleValue *) parameter_values);
    }
  }

  free_value(identifier_value);
  free_value(parameter_values);

  return result_value;
}


Value *evaluate_index_expression(Scope *scope, IndexExpression *index_expression) {
 return _evaluate_or_apply_index_operation(scope, index_expression, NULL);
}


Value *evaluate_infix_expression(Scope *scope, InfixExpression *infix_expression) {
  Value *left_value = NULL, *right_value = NULL, *result_value;

  Operator operator = infix_expression->operator;

  if (operator == ASSIGN_OP ||
    operator == ASSIGN_ADDITION_OP ||
    operator == ASSIGN_SUBTRACTION_OP ||
    operator == ASSIGN_MULTIPLICATION_OP ||
    operator == ASSIGN_DIVISION_OP ||
    operator == ASSIGN_INTEGER_DIVISION_OP ||
    operator == ASSIGN_EXPONENT_OP ||
    operator == ASSIGN_MOD_OP) {

    if (infix_expression->left_expression->expression_type == ExpressionTypeIdentifierExpression) {
      char *identifier = ((StringLiteral *) infix_expression->left_expression->literal)->string_literal;

      Variable *variable = scope_get_variable(scope, identifier);

      left_value = new_null_value();

      if (variable != NULL) {
        left_value = variable->value;
      }

      right_value = evaluate_expression(scope, infix_expression->right_expression);
      result_value = apply_assign_operation(left_value, right_value, operator);

      scope_set_variable(scope, identifier, result_value, false);

      if (left_value != NULL) free_value(left_value);
      free_value(right_value);

      return new_null_value();
    }
    else if (infix_expression->left_expression->expression_type == ExpressionTypeIndexExpression) {
      if (infix_expression->operator != ASSIGN_OP) {
        left_value = evaluate_expression(scope, infix_expression->left_expression);
      }

      right_value = evaluate_expression(scope, infix_expression->right_expression);
      result_value = apply_assign_operation(left_value, right_value, infix_expression->operator);

      free_value(apply_index_operation(scope, (IndexExpression *) infix_expression->left_expression, result_value));

      if (left_value != NULL) free_value(left_value);
      free_value(right_value);

      return new_null_value();
    }

    return new_null_value();
  }

  left_value = evaluate_expression(scope, infix_expression->left_expression);
  right_value = evaluate_expression(scope, infix_expression->right_expression);

  switch (infix_expression->operator) {
    case ADDITION_OP: {
      result_value = apply_addition_operation(left_value, right_value);
      break;
    }

    case SUBTRACTION_OP: {
      result_value = apply_subtraction_operation(left_value, right_value);
      break;
    }

    case MULTIPLICATION_OP: {
      result_value = apply_multiplication_operation(left_value, right_value);
      break;
    }

    case DIVISION_OP: {
      result_value = apply_division_operation(left_value, right_value, false);
      break;
    }

    case INTEGER_DIVISION_OP: {
      result_value = apply_division_operation(left_value, right_value, true);
      break;
    }

    case EXPONENT_OP: {
      result_value = apply_exponent_operation(left_value, right_value);
      break;
    }

    case MOD_OP: {
      result_value = apply_mod_operation(left_value, right_value);
      break;
    }

    case CHECK_EQUALITY_OP: {
      result_value = apply_check_equality_operation(left_value, right_value);
      break;
    }

    case CHECK_NOT_EQUALITY_OP: {
      result_value = apply_check_not_equality_operation(left_value, right_value);
      break;
    }

    case CHECK_BIGGER_OP: {
      result_value = apply_check_bigger_operation(left_value, right_value);
      break;
    }

    case CHECK_BIGGER_EQUAL_OP: {
      result_value = apply_check_bigger_equal_operation(left_value, right_value);
      break;
    }

    case CHECK_SMALLER_OP: {
      result_value = apply_check_smaller_operation(left_value, right_value);
      break;
    }

    case CHECK_SMALLER_EQUAL_OP: {
      result_value = apply_check_smaller_equal_operation(left_value, right_value);
      break;
    }

    default: {
      result_value = new_null_value();
    }
  }

  free_value(left_value);
  free_value(right_value);

  return result_value;
}


Value *evaluate_prefix_expression(Scope *scope, PrefixExpression *prefix_expression) {
  Value *result_value;

  Value *right_value = evaluate_expression(scope, prefix_expression->right_expression);

  switch (prefix_expression->operator) {
    case NOT_OP: {
      result_value = apply_prefix_not_operation(right_value);
      break;
    }

    case ADDITION_OP: {
      result_value = apply_prefix_plus_operation(right_value);
      break;
    }

    case SUBTRACTION_OP: {
      result_value = apply_prefix_minus_operation(right_value);
      break;
    }

    default: {
      result_value = new_null_value();
      break;
    }
  }

  free_value(right_value);

  return result_value;
}


Value *evaluate_expression(Scope *scope, Expression *expression) {
  switch (expression->expression_type) {
    case ExpressionTypeNullExpression: {
      return new_null_value();
    }

    case ExpressionTypeBoolExpression: {
      return new_bool_value(((BoolLiteral *) expression->literal)->bool_literal);
    }

    case ExpressionTypeIntegerExpression: {
      return new_integer_value_from_literal((IntegerLiteral *) expression->literal);
    }

    case ExpressionTypeFloatExpression: {
      return new_float_value_from_literal((FloatLiteral *) expression->literal);
    }

    case ExpressionTypeStringExpression: {
      return new_string_value_from_literal((StringLiteral *) expression->literal);
    }

    case ExpressionTypeInfixExpression: {
      return evaluate_infix_expression(scope, (InfixExpression *) expression);
    }

    case ExpressionTypePrefixExpression: {
      return evaluate_prefix_expression(scope, (PrefixExpression *) expression);
    }

    case ExpressionTypeIndexExpression: {
      IndexExpression *index_expression = (IndexExpression *) expression;

      return evaluate_index_expression(scope, index_expression);
    }

    case ExpressionTypeArrayExpression: {
      ArrayExpression *array_expression = (ArrayExpression *) expression;

      Value **items = malloc(array_expression->expression_count * sizeof(Value *));

      for (size_t i = 0; i < array_expression->expression_count; i++) {
        items[i] = evaluate_expression(scope, array_expression->expressions[i]);
        items[i]->linked_variable_count++;
      }

      if (array_expression->array_expression_type == ArrayExpressionTypeTuple) {
        return new_tuple_value(items, array_expression->expression_count, array_expression->has_finished);
      }
      else if (array_expression->array_expression_type == ArrayExpressionTypeList) {
        return new_list_value(items, array_expression->expression_count, array_expression->has_finished);
      }
    }

    case ExpressionTypeIdentifierExpression: {
      StringLiteral *string_literal = (StringLiteral *) expression->literal;

      Variable *variable = scope_get_variable(scope, string_literal->string_literal);

      if (variable == NULL) {
        return new_null_value();
      }

      return variable->value;
    }

    case ExpressionTypeCallExpression: {
      return evaluate_call_expression(scope, (CallExpression *) expression);
    }

    case ExpressionTypeMemberExpression: {
      MemberExpression *member_expression = (MemberExpression *) expression;

      Scope *current_scope = scope;
      Value *value = NULL;

      for (size_t i = 0; i < member_expression->expression_count; i++) {
        value = evaluate_expression(current_scope, member_expression->expressions[i]);

        if (value->value_type == ValueTypeObjectValue) {
          current_scope = ((ObjectValue *) value)->scope;
        }
        else {
          // TODO give error
          break;
        }
      }

      return value;
    }

    case ExpressionTypeFunctionExpression: {
      FunctionExpression *function_expression = (FunctionExpression *) expression;

      FunctionValue *function_value = (FunctionValue *) new_function_value(function_expression->block, function_expression->arguments, function_expression->argument_count);

      if (function_expression->identifier != NULL) {
        char *function_name = ((StringLiteral *) function_expression->identifier->literal)->string_literal;

        scope_set_variable(scope, function_name, (Value *) function_value, false);
      }

      return (Value *) function_value;
    }
  }

  return new_null_value();
}

// return value means if true move on
bool evaluate_block_definition(Scope *scope, BlockDefinition *block_definition) {
  Scope *block_scope;

  switch (block_definition->block_definition_type) {
    case BlockDefinitionTypeIfElseGroupBlock: {
      IfElseGroupBlockDefinition *if_else_group_block_definition = (IfElseGroupBlockDefinition *) block_definition;

      size_t i;

      for (i = 0; i < if_else_group_block_definition->if_block_definitions_length; i++) {
        if (evaluate_block_definition(scope, (BlockDefinition *) if_else_group_block_definition->if_block_definitions[i])) {
          break;
        }
      }

      if (i == if_else_group_block_definition->if_block_definitions_length && if_else_group_block_definition->else_block_definition != NULL) {
        evaluate_block_definition(scope, (BlockDefinition *) if_else_group_block_definition->else_block_definition);
      }

      return true;
    }

    case BlockDefinitionTypeIfBlock: {
      IfBlockDefinition *if_block_definition = (IfBlockDefinition *) block_definition;

      block_scope = new_scope(scope, if_block_definition->block, ScopeTypeNormalScope);

      if (if_block_definition->pre_expression != NULL) {
        free_value(evaluate_expression(block_scope, if_block_definition->pre_expression));
      }

      Value *condition_value = evaluate_expression(block_scope, if_block_definition->condition);
      bool condition = convert_to_bool(condition_value);

      if (condition) {
        free_value(evaluate_scope(block_scope));
      }

      free_value(condition_value);
      free_scope(block_scope);

      return condition;
    }

    case BlockDefinitionTypeElseBlock: {
      ElseBlockDefinition *else_block_definition = (ElseBlockDefinition *) block_definition;

      block_scope = new_scope(scope, else_block_definition->block, ScopeTypeNormalScope);

      free_value(evaluate_scope(block_scope));
      free_scope(block_scope);

      return true;
    }

    case BlockDefinitionTypeForBlock: {
      ForBlockDefinition *for_block_definition = (ForBlockDefinition *) block_definition;

      block_scope = new_scope(scope, for_block_definition->block, ScopeTypeNormalScope);

      if (for_block_definition->pre_expression != NULL) {
        free_value(evaluate_expression(block_scope, for_block_definition->pre_expression));
      }

      Value *condition_value = evaluate_expression(block_scope, for_block_definition->condition);

      while (convert_to_bool(condition_value)) {
        free_value(condition_value);

        free_value(evaluate_scope(block_scope));

        if (for_block_definition->post_expression != NULL) {
          free_value(evaluate_expression(block_scope, for_block_definition->post_expression));
        }

        condition_value = evaluate_expression(block_scope, for_block_definition->condition);
      }

      free_value(condition_value);
      free_scope(block_scope);

      return true;
    }

    default: {
      return true;
    }
  }
}


bool evaluate_statement(Scope *scope, Statement *statement, bool print_if_not_null) {
  switch (statement->statement_type) {
    case StatementTypeExpressionStatement: {
      Value *value = evaluate_expression(scope, ((ExpressionStatement *) statement)->expression);

      if (print_if_not_null && value->value_type != ValueTypeNullValue) {
        char *s = convert_to_string(value);
        printf("%s\n", s);
        free(s);
      }

      free_value(value);

      return true;
    }

    case StatementTypeReturnStatement: {
      scope_set_return_value(scope, evaluate_expression(scope, ((ReturnStatement *) statement)->right_expression));

      return false;
    }

    case StatementTypeImportStatement: {
      ImportStatement *import_statement = (ImportStatement *) statement;

      return true;
    }

    case StatementTypeBlockDefinitionStatement: {
      BlockDefinitionStatement *block_definition_statement = (BlockDefinitionStatement *) statement;

      BlockDefinition *block_definition = block_definition_statement->block_definition;

      evaluate_block_definition(scope, block_definition);

      return true;
    }

    default: {
      return true;
    }
  }
}


Value *evaluate_scope(Scope *scope) {
  for (size_t i = 0; i < scope->block->statement_count; i++) {
    if (!evaluate_statement(scope, scope->block->statements[i], false)) break;
  }

  return scope->return_value;
}


void evaluate_ast(AST *ast) {
  Scope *main_scope = new_scope(NULL, ast->block, ScopeTypeNormalScope);

  build_main_scope(main_scope);

  free_value(evaluate_scope(main_scope));

  free_scope(main_scope);
}