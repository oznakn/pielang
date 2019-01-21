#include "parser.h"

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"
#include "ast.h"

bool has_finished(Token token, ParserLimiter limiter) {
  if (token.token_type == EOF_TOKEN || token.token_type == EOL_TOKEN) {
    return true;
  }

  if (limiter == TUPLE_EXPRESSION_PARSER_LIMITER) {
    if (token.token_type == COMMA_TOKEN || token.token_type == R_PARENTHESIS_TOKEN) {
      return true;
    }
  }

  return false;
}

void *parser_error() {
  printf("ERROR\n");
  return NULL;
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

/* Expression *parse_grouped_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2) {
  Expression *result = parse_expression(lexer, 0, R_PARENTHESIS_TOKEN, until2);

  if (peek_token(lexer).token_type != R_PARENTHESIS_TOKEN) {
    return parser_error();
  }

  next_token(lexer);
  return result;
} */

Expression *parse_prefix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, limiter)) return parser_error();

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_tuple_expression(lexer, limiter, true);
  }

  PrefixExpression *prefix_expression = malloc(sizeof(PrefixExpression));

  prefix_expression->expression = (Expression){.expression_type = ExpressionTypePrefixExpression};
  prefix_expression->operator = token_to_operator(curr_token);
  prefix_expression->right_expression = parse_expression(lexer, PREFIX_PRECEDENCE, limiter);

  return (Expression *)prefix_expression;
}

Expression *parse_postfix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, limiter)) return left;

  PostfixExpression *postfixExpression = malloc(sizeof(PostfixExpression));

  postfixExpression->expression = (Expression){.expression_type = ExpressionTypePostfixExpression};
  postfixExpression->left_expression = left;
  postfixExpression->operator = token_to_operator(curr_token);

  return (Expression *)postfixExpression;
}

Expression *parse_tuple_expression(Lexer *lexer, ParserLimiter limiter, bool reduce) {
  TupleExpression *tuple_expression = malloc(sizeof(TupleExpression));

  tuple_expression->expression = (Expression){.expression_type = ExpressionTypeTupleExpression};
  tuple_expression->expression_count = 0;
  tuple_expression->expressions = malloc(tuple_expression->expression_count);

  while (peek_token(lexer).token_type != R_PARENTHESIS_TOKEN) {
    if (peek_token(lexer).token_type == COMMA_TOKEN) {
      next_token(lexer);
    }

    if (has_finished(peek_token(lexer), limiter)) return parser_error();

    Expression *expression = parse_expression(lexer, 0, TUPLE_EXPRESSION_PARSER_LIMITER);

    Expression **tmp = realloc(tuple_expression->expressions, sizeof(TupleExpression *) * (++tuple_expression->expression_count));

    if (tmp == NULL) {
      free(tuple_expression);
      return parser_error();
    }
    tuple_expression->expressions = tmp;

    tuple_expression->expressions[tuple_expression->expression_count - 1] = expression;
  }

  next_token(lexer);

  if (reduce && tuple_expression->expression_count == 1) {
    Expression *expression = tuple_expression->expressions[0];

    free(tuple_expression->expressions);
    free(tuple_expression);

    return expression;
  }

  return (Expression *)tuple_expression;
}

Expression *parse_call_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left) {
  CallExpression *call_expression = malloc(sizeof(CallExpression));
  call_expression->expression = (Expression){.expression_type = ExpressionTypeCallExpression};
  call_expression->identifier_expression = left;
  call_expression->tuple_expression = parse_tuple_expression(lexer, limiter, false);

  return (Expression *)call_expression;
}

Expression *parse_infix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, limiter)) return left;

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_call_expression(lexer, precedence, limiter, left);
  }

  InfixExpression *infix_expression = malloc(sizeof(InfixExpression));
  infix_expression->expression = (Expression){.expression_type = ExpressionTypeInfixExpression};
  infix_expression->left_expression = left;
  infix_expression->operator = token_to_operator(curr_token);
  infix_expression->right_expression = parse_expression(lexer, get_operator_precedence(infix_expression->operator, true), limiter);

  return (Expression *)infix_expression;
}

Expression *parse_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter) {
  Token curr_token = peek_token(lexer);

  if (has_finished(curr_token, limiter)) return parser_error();

  Expression *left;

  if (check_if_token_is_operator(curr_token)) {
    left = parse_prefix_expression(lexer, precedence, limiter);
  } else {
    left = eval_token(next_token(lexer));
  }

  curr_token = peek_token(lexer);
  if (has_finished(curr_token, limiter)) return left;

  while (precedence < get_operator_precedence(token_to_operator(curr_token), false)) {
    if (has_finished(curr_token, limiter)) {
      return left;
    } else if (check_if_token_is_postfix_operator(curr_token)) {
      left = parse_postfix_expression(lexer, precedence, limiter, left);
    }

    curr_token = peek_token(lexer);

    if (has_finished(curr_token, limiter)) {
      return left;
    } else if (check_if_token_is_operator(curr_token)) {
      left = parse_infix_expression(lexer, precedence, limiter, left);
      curr_token = peek_token(lexer);
    } else {
      return left;
    }
  }

  return left;
}

Statement *parse_print_statement(Lexer *lexer) {
  next_token(lexer);

  PrintStatement *print_statement = malloc(sizeof(PrintStatement));
  print_statement->statement = (Statement){.statement_type = StatementTypePrintStatement};
  print_statement->right_expression = parse_expression(lexer, 0, DEFAULT_EXPRESSION_PARSER_LIMITER);

  return (Statement *)print_statement;
}

Statement *parse_return_statement(Lexer *lexer) {
  next_token(lexer);

  ReturnStatement *return_statement = malloc(sizeof(ReturnStatement));
  return_statement->statement = (Statement){.statement_type = StatementTypeReturnStatement};
  return_statement->right_expression = parse_expression(lexer, 0, DEFAULT_EXPRESSION_PARSER_LIMITER);

  return (Statement *)return_statement;
}

Statement *parse_import_statement(Lexer *lexer) {
  next_token(lexer);

  ImportStatement *import_statement = malloc(sizeof(ImportStatement));
  import_statement->statement = (Statement){.statement_type = StatementTypeImportStatement};
  import_statement->right_expression = parse_expression(lexer, 0, DEFAULT_EXPRESSION_PARSER_LIMITER);

  return (Statement *)import_statement;
}

Statement *parse_expression_statement(Lexer *lexer) {
  Expression *expression = parse_expression(lexer, 0, DEFAULT_EXPRESSION_PARSER_LIMITER);

  ExpressionStatement *expression_statement = malloc(sizeof(ExpressionStatement));
  expression_statement->statement = (Statement){.statement_type = StatementTypeExpressionStatement};
  expression_statement->expression = expression;

  next_token(lexer);

  return (Statement *)expression_statement;
}

Statement *parse_statement(Lexer *lexer) {
  Token token = peek_token(lexer);

  Statement *statement;

  if (token.token_type == PRINT_TOKEN) {
    statement = parse_print_statement(lexer);
  } else if (token.token_type == RETURN_TOKEN) {
    statement = parse_return_statement(lexer);
  } else if (token.token_type == IMPORT_TOKEN) {
    statement = parse_import_statement(lexer);
  } else {
    statement = parse_expression_statement(lexer);
  }

  printf_statement(statement);

  return statement;
}

AST *parse_ast(Lexer *lexer) {
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

    ast->statements[ast->statement_count - 1] = parse_statement(lexer);
  }

  return ast;
}
