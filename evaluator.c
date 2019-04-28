#include "evaluator.h"

#include <string.h>
#include "bool.h"
#include "ast.h"
#include "scope.h"
#include "value.h"
#include "system.h"


Value *evaluate_infix_expression(Scope *scope, InfixExpression *infix_expression) {
  Value *result_value;

  Value *left_value = evaluate_expression(scope, infix_expression->left_expression);
  Value *right_value = evaluate_expression(scope, infix_expression->right_expression);

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
      result_value = apply_division_operation(left_value, right_value);
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

    default: {
      result_value = new_null_value();
      break;
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
    case ExpressionTypeIntegerExpression: {
      IntegerLiteral *integer_literal = (IntegerLiteral *) expression->literal;

      return new_integer_value_from_literal(integer_literal);
    }

    case ExpressionTypeFloatExpression: {
      FloatLiteral *float_literal = (FloatLiteral *) expression->literal;

      return new_float_value_from_literal(float_literal);
    }

    case ExpressionTypeStringExpression: {
      StringLiteral *string_literal = (StringLiteral *) expression->literal;

      return new_string_value_from_literal(string_literal);
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
      InfixExpression *infix_expression = (InfixExpression *) expression;

      return evaluate_infix_expression(scope, infix_expression);
    }

    case ExpressionTypePrefixExpression: {
      PrefixExpression *prefix_expression = (PrefixExpression *) expression;

      return evaluate_prefix_expression(scope, prefix_expression);
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

      evaluate_expression(scope, expression_statement->expression);
      break;
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

  char *variable_name = calloc(5, sizeof(char));
  char *variable_value = calloc(5, sizeof(char));

  strcpy(variable_name, "ozan");
  strcpy(variable_value, "akin");

  Value *value = new_string_value(variable_value, 4);
  Variable *variable = new_variable(variable_name, value);

  variable_map_set(main_scope->variable_map, variable);

  evaluate_scope(main_scope);

  free_scope(main_scope);
}