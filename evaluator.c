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
  Scope *function_scope = new_scope(scope, function_value->block, true);

  if (parameter_values->length > function_value->argument_count) return new_null_value();

  Value *variable_value;

  for (size_t i = 0; i < function_value->argument_count; i++) {
    if (parameter_values->length >= i) {
      variable_value = new_null_value();
    }
    else {
      variable_value = parameter_values->items[i];
    }

    scope_set_variable(function_scope, function_value->arguments[i], variable_value, 1, true);
  }

  return evaluate_scope(function_scope);
}


Value *apply_prefix_not_operation(Value *right_value) {
  if (right_value->value_type == ValueTypeBoolValue) {
    return new_bool_value(!((BoolValue *) right_value)->bool_value);
  }
  else if (right_value->value_type == ValueTypeIntegerValue) {
    return new_integer_value(!((IntegerValue *) right_value)->integer_value);
  }

  return new_null_value();
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

    return new_tuple_value(result_items, left_array_item_length + right_array_item_length, true);
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
  if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    if (left_value->value_type == ValueTypeIntegerValue && right_value->value_type == ValueTypeIntegerValue) {
      return new_integer_value(((IntegerValue *) left_value)->integer_value * ((IntegerValue *) right_value)->integer_value);
    }
    else {
      double left_float, right_float;

      if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
      else left_float = ((IntegerValue *) left_value)->integer_value;

      if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
      else right_float = ((IntegerValue *) right_value)->integer_value;

      return new_float_value(left_float * right_float);
    }
  }

  return new_null_value();
}


Value *apply_division_operation(Value *left_value, Value *right_value, bool is_integer_division) {
  if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    double left_float, right_float, result;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    result = left_float / right_float;

    if (is_integer_division || ceil(result) == floor(result)) {
      return new_integer_value((int) result);
    }
    else {
      return new_float_value(result);
    }
  }

  return new_null_value();
}


Value *apply_exponent_operation(Value *left_value, Value *right_value) {
  if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    double left_float, right_float, result;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    result = pow(left_float, right_float);

    if (ceil(result) == floor(result)) {
      return new_integer_value((int) result);
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


Value *evaluate_call_expression(Scope *scope, CallExpression *call_expression) {
  Value *identifier_value = evaluate_expression(scope, call_expression->identifier_expression);
  Value *parameter_values = evaluate_expression(scope, call_expression->tuple_expression);

  if (parameter_values->value_type == ValueTypeTupleValue) {
    if (identifier_value->value_type == ValueTypeFunctionValue) {
      return call_function(scope, (FunctionValue *) identifier_value, (TupleValue *) parameter_values);
    }
    if (identifier_value->value_type == ValueTypeSystemFunctionValue) {
      return call_system_function((SystemFunctionValue *) identifier_value, (TupleValue *) parameter_values);
    }
  }

  return new_null_value();
}


Value *evaluate_index_expression(Scope *scope, IndexExpression *index_expression) {
 return _evaluate_or_apply_index_operation(scope, index_expression, NULL);
}


Value *evaluate_infix_expression(Scope *scope, InfixExpression *infix_expression) {
  Value *left_value = NULL, *right_value = NULL, *result_value;

  switch (infix_expression->operator) {
    case ASSIGN_OP:
    case ASSIGN_ADDITION_OP:
    case ASSIGN_SUBTRACTION_OP:
    case ASSIGN_MULTIPLICATION_OP:
    case ASSIGN_DIVISION_OP:
    case ASSIGN_INTEGER_DIVISION_OP:
    case ASSIGN_EXPONENT_OP:
    case ASSIGN_MOD_OP: {
      if (infix_expression->left_expression->expression_type == ExpressionTypePrefixExpression && ((PrefixExpression *) infix_expression->left_expression)->operator == LET_OP) {
        PrefixExpression *let_expression = (PrefixExpression *) infix_expression->left_expression;

        if ( let_expression->right_expression->literal != NULL && let_expression->right_expression->literal->literal_type == LiteralTypeStringLiteral) {
          char *identifier = ((StringLiteral *) let_expression->right_expression->literal)->string_literal;

          right_value = evaluate_expression(scope, infix_expression->right_expression);
          result_value = apply_assign_operation(NULL, right_value, infix_expression->operator);

          scope_set_variable(scope, identifier, result_value, -1, true);

          free_value(right_value);
        }

        return new_null_value();
      }
      else if (infix_expression->left_expression->expression_type == ExpressionTypeIdentifierExpression) {
        char *identifier = ((StringLiteral *) infix_expression->left_expression->literal)->string_literal;
        Variable *variable = scope_get_variable(scope, identifier);

        if (variable != NULL) {
          left_value = variable->value;

          right_value = evaluate_expression(scope, infix_expression->right_expression);
          result_value = apply_assign_operation(left_value, right_value, infix_expression->operator);

          scope_set_variable(scope, identifier, result_value, 1, false);

          free_value(left_value);
          free_value(right_value);
        }

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

    case ADDITION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_addition_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);

      return result_value;
    }

    case SUBTRACTION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_subtraction_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);

      return result_value;
    }

    case MULTIPLICATION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_multiplication_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);

      return result_value;
    }

    case DIVISION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_division_operation(left_value, right_value, false);

      free_value(left_value);
      free_value(right_value);

      return result_value;
    }

    case INTEGER_DIVISION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_division_operation(left_value, right_value, true);

      free_value(left_value);
      free_value(right_value);

      return result_value;
    }

    case EXPONENT_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_exponent_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);

      return result_value;
    }

    case MOD_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_mod_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);

      return result_value;
    }

    default: {
      return new_null_value();
    }
  }
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
  }

  return new_null_value();
}


bool evaluate_statement(Scope *scope, Statement *statement) {
  switch (statement->statement_type) {
    case StatementTypeExpressionStatement: {
      free_value(evaluate_expression(scope, ((ExpressionStatement *) statement)->expression));

      return true;
    }

    case StatementTypeReturnStatement: {
      scope->return_value = evaluate_expression(scope, ((ReturnStatement *) statement)->right_expression);

      return false;
    }

    case StatementTypeImportStatement: {
      ImportStatement *import_statement = (ImportStatement *) statement;
      break;
    }

    case StatementTypeBlockDefinitionStatement: {
      BlockDefinitionStatement *block_definition_statement = (BlockDefinitionStatement *) statement;
      break;
    }

    default: {
      break;
    }
  }

  return false;
}


Value *evaluate_scope(Scope *scope) {
  for (size_t i = 0; i < scope->block->statement_count; i++) {
    if (!evaluate_statement(scope, scope->block->statements[i])) break;
  }

  return scope->return_value;
}


void evaluate_ast(AST *ast) {
  Scope *main_scope = new_scope(NULL, ast->block, false);

  build_main_scope(main_scope);

  free_value(evaluate_scope(main_scope));

  free_scope(main_scope);
}