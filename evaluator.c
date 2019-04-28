#include "evaluator.h"

#include <string.h>
#include <math.h>
#include "bool.h"
#include "ast.h"
#include "scope.h"
#include "value.h"


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


Value *apply_division_operation(Value *left_value, Value *right_value) {
  if ((left_value->value_type == ValueTypeIntegerValue || left_value->value_type == ValueTypeFloatValue) && (right_value->value_type == ValueTypeIntegerValue || right_value->value_type == ValueTypeFloatValue)) {
    double left_float, right_float, result;

    if (left_value->value_type == ValueTypeFloatValue) left_float = ((FloatValue *) left_value)->float_value;
    else left_float = ((IntegerValue *) left_value)->integer_value;

    if (right_value->value_type == ValueTypeFloatValue) right_float = ((FloatValue *) right_value)->float_value;
    else right_float = ((IntegerValue *) right_value)->integer_value;

    result = left_float / right_float;

    if (ceil(result) == floor(result)) {
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


Value *apply_assign_operation(Scope *scope, char *identifier, Value *right_value) {
  set_variable(scope, identifier, right_value);

  return new_null_value();
}


Value *evaluate_infix_expression(Scope *scope, InfixExpression *infix_expression) {
  Value *left_value, *right_value, *result_value;

  switch (infix_expression->operator) {
    case ASSIGN_OP: {
      if (infix_expression->left_expression->literal->literal_type != LiteralTypeStringLiteral) {
        result_value = new_null_value();
      }
      else {
        char *identifier = ((StringLiteral *) infix_expression->left_expression->literal)->string_literal;

        right_value = evaluate_expression(scope, infix_expression->right_expression);

        result_value = apply_assign_operation(scope, identifier, right_value);

        free_value(right_value);
      }
      break;
    }

    case ADDITION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_addition_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);
      break;
    }

    case SUBTRACTION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_subtraction_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);
      break;
    }

    case MULTIPLICATION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_multiplication_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);
      break;
    }

    case DIVISION_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_division_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);
      break;
    }

    case EXPONENT_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_exponent_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);
      break;
    }

    case MOD_OP: {
      left_value = evaluate_expression(scope, infix_expression->left_expression);
      right_value = evaluate_expression(scope, infix_expression->right_expression);

      result_value = apply_mod_operation(left_value, right_value);

      free_value(left_value);
      free_value(right_value);
      break;
    }

    default: {
      result_value = new_null_value();
      break;
    }
  }

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

    case ExpressionTypeArrayExpression: {
      ArrayExpression *array_expression = (ArrayExpression *) expression;

      Value **items = malloc(array_expression->expression_count * sizeof(Value *));

      for (size_t i = 0; i < array_expression->expression_count; i++) {
        items[i] = evaluate_expression(scope, array_expression->expressions[i]);
        items[i]->linked_variable_count++;
      }

      if (array_expression->array_expression_type == ArrayExpressionTypeTuple) {
        return new_tuple_value(items, array_expression->expression_count);
      }
      else if (array_expression->array_expression_type == ArrayExpressionTypeList) {
        return new_list_value(items, array_expression->expression_count);
      }
    }

    case ExpressionTypeIdentifierExpression: {
      StringLiteral *string_literal = (StringLiteral *) expression->literal;

      Variable *variable = get_variable(scope, string_literal->string_literal);

      if (variable == NULL) {
        return new_null_value();
      }

      return variable->value;
    }

    case ExpressionTypeInfixExpression: {
      return evaluate_infix_expression(scope, (InfixExpression *) expression);
    }

    case ExpressionTypePrefixExpression: {
      return evaluate_prefix_expression(scope, (PrefixExpression *) expression);
    }
  }

  return new_null_value();
}


bool evaluate_print_statement(Scope *scope, PrintStatement *print_statement) {
  Value *value = evaluate_expression(scope, print_statement->right_expression);

  StringValue *string_value = (StringValue *) convert_to_string_value(value);

  printf("%s\n", string_value->string_value);

  free_value((Value *) string_value);
  free_value(value);

  return true;
}


bool evaluate_statement(Scope *scope, Statement *statement) {
  switch (statement->statement_type) {
    case StatementTypePrintStatement: {
      PrintStatement *print_statement = (PrintStatement *) statement;

      return evaluate_print_statement(scope, print_statement);
    }

    case StatementTypeExpressionStatement: {
      ExpressionStatement *expression_statement = (ExpressionStatement *) statement;

      free_value(evaluate_expression(scope, expression_statement->expression));

      return true;
    }

    case StatementTypeReturnStatement: {
      ReturnStatement *return_statement = (ReturnStatement *) statement;
      break;
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


void evaluate_scope(Scope *scope) {
  for (size_t i = 0; i < scope->block->statement_count; i++) {
    if (!evaluate_statement(scope, scope->block->statements[i])) break;
  }
}


void evaluate_ast(AST *ast) {
  Scope *main_scope = new_scope(NULL, ast->block);

  evaluate_scope(main_scope);

  free_scope(main_scope);
}