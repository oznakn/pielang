#include "ast.h"

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"

void print_expression(Expression *expression) {
  if (expression->expression_type == ExpressionTypeInfixExpression) {
    InfixExpression *infix_expression = (InfixExpression *)expression;

    printf(" (");
    print_expression(infix_expression->left_expression);

    if (infix_expression->operator == ADDITION_OP) { printf(" + "); }
    else if (infix_expression->operator == SUBTRACTION_OP) { printf(" - "); }
    else if (infix_expression->operator == MULTIPLICATION_OP) { printf(" * "); }
    else if (infix_expression->operator == DIVISION_OP) { printf(" / "); }
    else if (infix_expression->operator == MOD_OP) { printf(" %% "); }
    else if (infix_expression->operator == EXPONENT_OP) { printf(" ^ "); }
    else if (infix_expression->operator == ASSIGN_OP) { printf(" = "); }
    else if (infix_expression->operator == MEMBER_OP) { printf(" . "); }

    print_expression(infix_expression->right_expression);
    printf(") ");
  } else if (expression->expression_type == ExpressionTypePrefixExpression) {
    PrefixExpression *prefix_expression = (PrefixExpression *)expression;

    printf(" (");

    if (prefix_expression->operator == ADDITION_OP) { printf(" +"); }
    else if (prefix_expression->operator == SUBTRACTION_OP) { printf(" -"); }
    else if (prefix_expression->operator == PLUS_PLUS_OP) { printf("++"); }
    else if (prefix_expression->operator == MINUS_MINUS_OP) { printf("--"); }

    print_expression(prefix_expression->right_expression);
    printf(") ");

  } else if (expression->expression_type == ExpressionTypePostfixExpression) {
    PostfixExpression *postfix_expression = (PostfixExpression *)expression;

    printf(" (");
    print_expression(postfix_expression->left_expression);

    if (postfix_expression->operator == PLUS_PLUS_OP) { printf("++"); }
    else if (postfix_expression->operator == MINUS_MINUS_OP) { printf("--"); }

    printf(") ");

  } else if (expression->expression_type == ExpressionTypeCallExpression) {
    CallExpression *call_expression = (CallExpression *)expression;

    printf(" `");
    print_expression(call_expression->identifier_expression);
    printf("->");
    print_expression(call_expression->tuple_expression);
    printf("` ");
  } else if (expression->expression_type == ExpressionTypeTupleExpression) {
    TupleExpression *tuple_expression = (TupleExpression *)expression;

    printf(" (");
    for (size_t i = 0; i < tuple_expression->expression_count; i++) {
      print_expression(tuple_expression->expressions[i]);

      if (i < tuple_expression->expression_count - 1) {
        printf(",");
      }
    }
    printf(") ");
  } else if (expression->expression_type == ExpressionTypeIntegerExpression) {
    printf(" %lu ", expression->value.integer_value);
  } else if (expression->expression_type == ExpressionTypeFloatExpression) {
    printf(" %f ", expression->value.float_value);
  } else if (expression->expression_type == ExpressionTypeStringExpression) {
    printf(" \"%s\" ", expression->value.string_value);
  } else if (expression->expression_type == ExpressionTypeBoolExpression) {
    printf(" %s ", expression->value.bool_value ? "true" : "false");
  } else if (expression->expression_type == ExpressionTypeIdentifierExpression) {
    printf(" %s ", expression->value.string_value);
  }
}

void free_expression(Expression *expression) {
  switch (expression->expression_type) {
    case ExpressionTypeInfixExpression: {
      InfixExpression *infix_expression = (InfixExpression *)expression;

      free_expression(infix_expression->left_expression);
      free_expression(infix_expression->right_expression);
      free(infix_expression);

      break;
    }

    case ExpressionTypePrefixExpression: {
      PrefixExpression *prefix_expression = (PrefixExpression *)expression;

      free_expression(prefix_expression->right_expression);
      free(prefix_expression);

      break;
    }

    case ExpressionTypePostfixExpression: {
      PostfixExpression *postfix_expression = (PostfixExpression *)expression;

      free_expression(postfix_expression->left_expression);
      free(postfix_expression);

      break;
    }

    case ExpressionTypeCallExpression: {
      CallExpression *call_expression = (CallExpression *)expression;

      free_expression(call_expression->identifier_expression);
      free_expression(call_expression->tuple_expression);
      free(call_expression);

      break;
    }

    case ExpressionTypeTupleExpression: {
      TupleExpression *tuple_expression = (TupleExpression *)expression;

      for (size_t i = 0; i < tuple_expression->expression_count; i++) {
        free_expression(tuple_expression->expressions[i]);
      }

      free(tuple_expression->expressions);
      free(tuple_expression);

      break;
    }

    case ExpressionTypeIntegerExpression: {
      free(expression);
      break;
    }

    case ExpressionTypeFloatExpression: {
      free(expression);
      break;
    }

    case ExpressionTypeStringExpression: {
      free(expression->value.string_value);
      free(expression);
      break;
    }

    case ExpressionTypeIdentifierExpression: {
      free(expression->value.string_value);
      free(expression);
      break;
    }

    default: {

    }
  }
}

void free_statement(Statement *statement) {
  switch (statement->statement_type) {
    case StatementTypeExpressionStatement: {
      ExpressionStatement *expression_statement = (ExpressionStatement *)statement;

      free_expression(expression_statement->expression);
      free(expression_statement);

      break;
    }

    case StatementTypePrintStatement: {
      PrintStatement *print_statement = (PrintStatement *)statement;

      free_expression(print_statement->right_expression);
      free(print_statement);
      break;
    }

    default: {

    }
  }
}

void free_ast(AST *ast) {
  for (size_t i = 0; i < ast->statement_count; i++) {
    free_statement(ast->statements[i]);
  }
  free(ast);
}

Operator token_to_operator(Token token) {
  switch (token.token_type) {
    case MEMBER_TOKEN:
      return MEMBER_OP;

    case ASSIGN_TOKEN:
      return ASSIGN_OP;

    case ADDITION_TOKEN:
      return ADDITION_OP;

    case ASSIGN_ADDITION_TOKEN:
      return ASSIGN_ADDITION_OP;

    case SUBTRACTION_TOKEN:
      return SUBTRACTION_OP;

    case ASSIGN_SUBTRACTION_TOKEN:
      return ASSIGN_SUBTRACTION_OP;

    case MULTIPLICATION_TOKEN:
      return MULTIPLICATION_OP;

    case ASSIGN_MULTIPLICATION_TOKEN:
      return ASSIGN_MULTIPLICATION_OP;

    case DIVISION_TOKEN:
      return DIVISION_OP;

    case ASSIGN_DIVISION_TOKEN:
      return ASSIGN_DIVISION_OP;

    case INTEGER_DIVISION_TOKEN:
      return INTEGER_DIVISION_OP;

    case ASSIGN_INTEGER_DIVISION_TOKEN:
      return ASSIGN_INTEGER_DIVISION_OP;

    case MOD_TOKEN:
      return MOD_OP;

    case ASSIGN_MOD_TOKEN:
      return ASSIGN_MOD_OP;

    case EXPONENT_TOKEN:
      return EXPONENT_OP;

    case ASSIGN_EXPONENT_TOKEN:
      return ASSIGN_EXPONENT_OP;

    case PLUS_PLUS_TOKEN:
      return PLUS_PLUS_OP;

    case MINUS_MINUS_TOKEN:
      return MINUS_MINUS_OP;

    case L_PARENTHESIS_TOKEN:
      return L_PARENTHESIS_OP;

    default:
      return -1;
  }
}

bool check_if_token_is_operator(Token token) {
  return token_to_operator(token) != -1;
}

bool is_right_associative(Token token) {
  return token.token_type == EXPONENT_TOKEN || token.token_type == ASSIGN_TOKEN;
}

bool check_if_token_is_postfix_operator(Token token) {
  return token.token_type == PLUS_PLUS_TOKEN || token.token_type == MINUS_MINUS_TOKEN;
}

unsigned short get_operator_precedence(Token token, bool next) {
  unsigned short result;

  switch (token.token_type) {
    case ASSIGN_TOKEN:
    case ASSIGN_ADDITION_TOKEN:
    case ASSIGN_SUBTRACTION_TOKEN:
    case ASSIGN_MULTIPLICATION_TOKEN:
    case ASSIGN_DIVISION_TOKEN:
    case ASSIGN_INTEGER_DIVISION_TOKEN:
    case ASSIGN_MOD_TOKEN:
    case ASSIGN_EXPONENT_TOKEN: {
      result = ASSIGN_PRECEDENCE;
      break;
    }

    case ADDITION_TOKEN:
    case SUBTRACTION_TOKEN: {
      result = ADDITION_SUBTRACTION_PRECEDENCE;
      break;
    };

    case MULTIPLICATION_TOKEN:
    case DIVISION_TOKEN:
    case INTEGER_DIVISION_TOKEN:
    case MOD_TOKEN: {
      result = MULTIPLICATION_DIVISION_MOD_PRECEDENCE;
      break;
    };

    case EXPONENT_TOKEN: {
      result = EXPONENT_PRECEDENCE;
      break;
    };

    case PLUS_PLUS_TOKEN:
    case MINUS_MINUS_TOKEN: {
      result = PREFIX_PRECEDENCE;
      break;
    };

    case L_PARENTHESIS_TOKEN: {
      result = L_PARENTHESIS_PRECEDENCE;
      break;
    };

    case MEMBER_TOKEN: {
      result = MEMBER_PRECEDENCE;
      break;
    };

    default: {
      result = 0;
    }
  }

  if (next && is_right_associative(token)) result -= 1u;

  return result;
}

bool has_finished(Token token, TokenType until1, TokenType until2) {
  return token.token_type == until1 || token.token_type == until2;
}
