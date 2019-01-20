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

    printf(" ` %s", call_expression->identifier);
    print_expression((Expression *)call_expression->tuple_expression);
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

      if (call_expression->identifier != NULL) free(call_expression->identifier);

      free_expression((Expression *)call_expression->tuple_expression);
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
  if (token.token_type == EXPONENT_TOKEN) return true;

  return false;
}

bool check_if_token_is_postfix_operator(Token token) {
  return token.token_type == PLUS_PLUS_TOKEN || token.token_type == MINUS_MINUS_TOKEN;
}

unsigned short get_operator_precedence(Token token, unsigned short curr_precedence) {
  /*
    *
    public
    static final int ASSIGNMENT = 1;
    public
    static final int CONDITIONAL = 2;
    public
    static final int SUM = 3;
    public
    static final int PRODUCT = 4;
    public
    static final int EXPONENT = 5;
    public
    static final int PREFIX = 6;
    public
    static final int POSTFIX = 7;
    public
    static final int CALL = 8;
  */


  switch (token.token_type) {
    case ASSIGN_TOKEN:
    case ASSIGN_ADDITION_TOKEN:
    case ASSIGN_SUBTRACTION_TOKEN:
    case ASSIGN_MULTIPLICATION_TOKEN:
    case ASSIGN_DIVISION_TOKEN:
    case ASSIGN_INTEGER_DIVISION_TOKEN:
    case ASSIGN_MOD_TOKEN:
    case ASSIGN_EXPONENT_TOKEN:
      return 1;

    case ADDITION_TOKEN:
    case SUBTRACTION_TOKEN:
      return 3;

    case MULTIPLICATION_TOKEN:
    case DIVISION_TOKEN:
    case INTEGER_DIVISION_TOKEN:
    case MOD_TOKEN:
      return 4;

    case EXPONENT_TOKEN:
      return 6;

    case PLUS_PLUS_TOKEN:
    case MINUS_MINUS_TOKEN:
      return 8;

    case L_PARENTHESIS_TOKEN:
      return 9;

    case MEMBER_OP:
      return 10;

    default:
      return 0;
  }
}

bool has_finished(Token token, TokenType until1, TokenType until2) {
  return token.token_type == until1 || token.token_type == until2;
}

Expression *eval_token(Token token) {
  Expression *expression = NULL;

  if (token.token_type == IDENTIFIER_TOKEN) {
    expression = malloc(sizeof(Expression));

    expression->value = (Value){.string_value = token.value.string_value};
    expression->expression_type = ExpressionTypeIdentifierExpression;

    return expression;
  } else if (token.token_type == INTEGER_TOKEN) {
    expression = malloc(sizeof(Expression));

    expression->value = (Value){.integer_value = token.value.integer_value};
    expression->expression_type = ExpressionTypeIntegerExpression;

    return expression;
  } else if (token.token_type == FLOAT_TOKEN) {
    expression = malloc(sizeof(Expression));

    expression->value = (Value){.float_value = token.value.float_value};
    expression->expression_type = ExpressionTypeFloatExpression;

    return expression;
  } else if (token.token_type == BOOL_TOKEN) {
    expression = malloc(sizeof(Expression));

    expression->value = (Value){.bool_value = token.value.bool_value};
    expression->expression_type = ExpressionTypeBoolExpression;

    return expression;
  } else if (token.token_type == STRING_LITERAL_TOKEN) {
    expression = malloc(sizeof(Expression));

    expression->value = (Value){.string_value = token.value.string_value};
    expression->expression_type = ExpressionTypeStringExpression;

    return expression;
  }

  return expression;
}

Expression *parse_grouped_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2) {
  Expression *result = parse_expression(lexer, 0, R_PARENTHESIS_TOKEN, until1);
  next_token(lexer);
  return result;
}

Expression *parse_prefix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, until1, until2)) return NULL;

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_grouped_expression(lexer, precedence, until1, until2);
  }

  PrefixExpression *prefix_expression = malloc(sizeof(PrefixExpression));

  prefix_expression->expression = (Expression){.expression_type = ExpressionTypePrefixExpression};
  prefix_expression->operator = token_to_operator(curr_token);
  prefix_expression->right_expression = parse_expression(lexer, 11, until1, until2); // TODO

  return (Expression *)prefix_expression;
}

Expression *parse_postfix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, until1, until2)) return left;

  PostfixExpression *postfixExpression = malloc(sizeof(PostfixExpression));

  postfixExpression->expression = (Expression){.expression_type = ExpressionTypePostfixExpression};
  postfixExpression->left_expression = left;
  postfixExpression->operator = token_to_operator(curr_token);

  return (Expression *)postfixExpression;
}

TupleExpression *parse_tuple_expression(Lexer *lexer) {
  TupleExpression *tuple_expression = malloc(sizeof(TupleExpression));

  tuple_expression->expression = (Expression){.expression_type = ExpressionTypeTupleExpression};
  tuple_expression->expression_count = 0;
  tuple_expression->expressions = malloc(tuple_expression->expression_count);

  while (peek_token(lexer).token_type != R_PARENTHESIS_TOKEN) {
    if (peek_token(lexer).token_type == COMMA_TOKEN) {
      next_token(lexer);
    }

    Expression *expression = parse_expression(lexer, 0, COMMA_TOKEN, R_PARENTHESIS_TOKEN);

    Expression **tmp = realloc(tuple_expression->expressions, sizeof(TupleExpression *) * (++tuple_expression->expression_count));

    if (tmp == NULL) {
      free(tuple_expression);
      return NULL;
    }
    tuple_expression->expressions = tmp;

    tuple_expression->expressions[tuple_expression->expression_count - 1] = expression;
  }
  next_token(lexer);

  return tuple_expression;
}

Expression *parse_call_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left) {
  CallExpression *call_expression = malloc(sizeof(CallExpression));
  call_expression->expression = (Expression){.expression_type = ExpressionTypeCallExpression};
  call_expression->identifier = left->value.string_value;
  call_expression->tuple_expression = parse_tuple_expression(lexer);

  return (Expression *)call_expression;
}

Expression *parse_infix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, until1, until2)) return left;

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_call_expression(lexer, precedence, until1, until2, left);
  }

  InfixExpression *infix_expression = malloc(sizeof(InfixExpression));
  infix_expression->expression = (Expression){.expression_type = ExpressionTypeInfixExpression};
  infix_expression->left_expression = left;
  infix_expression->operator = token_to_operator(curr_token);
  infix_expression->right_expression = parse_expression(lexer, get_operator_precedence(curr_token, precedence), until1, until2);

  return (Expression *)infix_expression;
}

Expression *parse_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2) {
  Token curr_token = peek_token(lexer);

  if (has_finished(curr_token, until1, until2)) return NULL;

  Expression *left;

  if (check_if_token_is_operator(curr_token)) {
    left = parse_prefix_expression(lexer, precedence, until1, until2);
  } else {
    left = eval_token(next_token(lexer));
  }

  curr_token = peek_token(lexer);
  if (has_finished(curr_token, until1, until2)) return left;

  while (precedence < get_operator_precedence(curr_token, precedence)) {
    curr_token = peek_token(lexer);
    if (has_finished(curr_token, until1, until2)) {
      return left;
    } else if (check_if_token_is_postfix_operator(curr_token)) {
      left = parse_postfix_expression(lexer, precedence, until1, until2, left);
    }

    curr_token = peek_token(lexer);

    if (has_finished(curr_token, until1, until2)) {
      return left;
    } else if (check_if_token_is_operator(curr_token)) {
      left = parse_infix_expression(lexer, precedence, until1, until2, left);
    } else {
      return left;
    }
  }

  return left;
}

AST *parse(Lexer *lexer) {
  AST *ast = malloc(sizeof(AST));
  ast->statement_count = 0;

  ast->statements = malloc(ast->statement_count);
  Statement **tmp = NULL;

  while (true) {
    Token token = peek_token(lexer);

    if (token.token_type == EOF_TOKEN) { break; }

    tmp = realloc(ast->statements, (++ast->statement_count) * sizeof(Statement *));
    if (tmp == NULL) {
      free(ast->statements);
      break;
    }
    ast->statements = tmp;

    if (true) {
      Expression *expression = parse_expression(lexer, 0, EOF_TOKEN, EOL_TOKEN);

      ExpressionStatement *expression_statement = malloc(sizeof(ExpressionStatement));
      expression_statement->statement = (Statement){.statement_type = StatementTypeExpressionStatement};
      expression_statement->expression = expression;

      print_expression(expression);

      ast->statements[ast->statement_count - 1] = (Statement *)expression_statement;
    }
  }

  return ast;
}


