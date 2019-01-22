#include "parser.h"

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"
#include "ast.h"

bool has_finished(Token token, ParserLimiter limiter) {
  if (token.token_type == EOF_TOKEN) {
    return true;
  }

  switch (limiter) {
    case DEFAULT_BLOCK_PARSER_LIMITER:
      return token.token_type == R_BRACE_TOKEN;

    case DEFAULT_EXPRESSION_PARSER_LIMITER:
      return token.token_type == EOL_TOKEN;

    case TUPLE_EXPRESSION_PARSER_LIMITER:
      return token.token_type == COMMA_TOKEN || token.token_type == R_PARENTHESIS_TOKEN || token.token_type == EOL_TOKEN;

    case GROUPED_EXPRESSION_PARSER_LIMITER:
      return token.token_type == R_PARENTHESIS_TOKEN;

    case DEFAULT_BLOCK_EXPRESSION_PARSER_LIMITER:
      return token.token_type == L_BRACE_TOKEN;

    case IF_BLOCK_EXPRESSION_PARSER_LIMITER:
    case FOR_BLOCK_EXPRESSION_PARSER_LIMITER:
      return token.token_type == L_BRACE_TOKEN || token.token_type == SEMICOLON_TOKEN;

    default:
      return false;
  }
}

void *parser_error() {
  printf("ERROR\n");
  return NULL;
}

Expression *eval_token(Token token) {
  Expression *expression;

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

  return parser_error();
}

Expression *parse_grouped_expression(Lexer *lexer) {
  Expression *result = parse_expression(lexer, 0, GROUPED_EXPRESSION_PARSER_LIMITER);

  if (peek_token(lexer).token_type != R_PARENTHESIS_TOKEN) {
    return parser_error();
  }

  next_token(lexer);
  return result;
}

Expression *parse_prefix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, limiter)) return parser_error();

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_grouped_expression(lexer); // TODO limiter, true
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

Expression *parse_tuple_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left) {
  TupleExpression *tuple_expression = NULL;

  if (left->expression_type != ExpressionTypeTupleExpression) {
    tuple_expression = malloc(sizeof(TupleExpression));

    tuple_expression->expression = (Expression){.expression_type = ExpressionTypeTupleExpression};
    tuple_expression->expression_count = 1;
    tuple_expression->expressions = malloc(tuple_expression->expression_count);
    tuple_expression->expressions[0] = left;
  } else {
    tuple_expression = (TupleExpression *)left;
  }

  if (has_finished(peek_token(lexer), limiter)) return (Expression *)tuple_expression;

  Expression *right = parse_expression(lexer, get_operator_precedence(COMMA_OP, true), limiter);

  Expression **tmp = realloc(tuple_expression->expressions, sizeof(Expression *) * (++tuple_expression->expression_count));
  if (tmp == NULL) {
    free_expression(left);
    return NULL;
  }
  tuple_expression->expressions = tmp;

  tuple_expression->expressions[tuple_expression->expression_count - 1] = right;

  return (Expression *)tuple_expression;
}

Expression *parse_call_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left) {
  CallExpression *call_expression = malloc(sizeof(CallExpression));
  call_expression->expression = (Expression){.expression_type = ExpressionTypeCallExpression};
  call_expression->identifier_expression = left;
  call_expression->tuple_expression = parse_grouped_expression(lexer); // TODO limiter, false

  return (Expression *)call_expression;
}

Expression *parse_infix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left) {
  Token curr_token = next_token(lexer);
  if (has_finished(curr_token, limiter)) return left;

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_call_expression(lexer, precedence, limiter, left);
  } else if (curr_token.token_type == COMMA_TOKEN) {
    return parse_tuple_expression(lexer, 0, TUPLE_EXPRESSION_PARSER_LIMITER, left);
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

Block *parse_block(Lexer *lexer, ParserLimiter limiter) {
  Block *block = malloc(sizeof(Block));

  block->statement_count = 0;
  block->statements = malloc(block->statement_count);
  Statement **tmp = NULL;

  while (true) {
    while (peek_token(lexer).token_type == EOL_TOKEN) { next_token(lexer); }

    Token token = peek_token(lexer);

    if (has_finished(token, limiter)) { break; }

    tmp = realloc(block->statements, (++block->statement_count) * sizeof(Statement *));
    if (tmp == NULL) {
      free(block->statements);
      break;
    }
    block->statements = tmp;

    block->statements[block->statement_count - 1] = parse_statement(lexer);
  }

  return block;
}

BlockDefinition *parse_if_block_definition(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  Expression *expression1 = NULL;
  Expression *expression2 = NULL;

  expression1 = parse_expression(lexer, 0, IF_BLOCK_EXPRESSION_PARSER_LIMITER);

  if (peek_token(lexer).token_type == SEMICOLON_TOKEN) {
    next_token(lexer);

    expression2 = parse_expression(lexer, 0, IF_BLOCK_EXPRESSION_PARSER_LIMITER);
  }

  if (peek_token(lexer).token_type != L_BRACE_TOKEN) return parser_error();

  next_token(lexer);

  Block *block = parse_block(lexer, limiter);

  if (peek_token(lexer).token_type != R_BRACE_TOKEN) return parser_error();

  next_token(lexer);

  IfBlockDefinition *if_block_definition = malloc(sizeof(IfBlockDefinition));
  if_block_definition->block_definition = (BlockDefinition){.block_definition_type = BlockDefinitionTypeIfBlock, .block = block};

  if (expression2 == NULL) {
    if_block_definition->condition = expression1;
  } else {
    if_block_definition->pre_expression = expression1;
    if_block_definition->condition = expression2;
  }

  return (BlockDefinition *)if_block_definition;
}

BlockDefinition *parse_for_block_definition(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  Expression *expression1 = NULL;
  Expression *expression2 = NULL;
  Expression *expression3 = NULL;

  expression1 = parse_expression(lexer, 0, FOR_BLOCK_EXPRESSION_PARSER_LIMITER);

  if (peek_token(lexer).token_type == SEMICOLON_TOKEN) {
    next_token(lexer);

    expression2 = parse_expression(lexer, 0, FOR_BLOCK_EXPRESSION_PARSER_LIMITER);

    if (peek_token(lexer).token_type == SEMICOLON_TOKEN) {
      next_token(lexer);

      expression3 = parse_expression(lexer, 0, FOR_BLOCK_EXPRESSION_PARSER_LIMITER);
    }
  }

  printf("%d\n", peek_token(lexer).token_type);
  if (peek_token(lexer).token_type != L_BRACE_TOKEN) return parser_error();

  next_token(lexer);

  Block *block = parse_block(lexer, limiter);

  if (peek_token(lexer).token_type != R_BRACE_TOKEN) return parser_error();

  next_token(lexer);

  ForBlockDefinition *for_block_definition = malloc(sizeof(ForBlockDefinition));
  for_block_definition->block_definition = (BlockDefinition){.block_definition_type = BlockDefinitionTypeForBlock, .block = block};

  if (expression2 == NULL) {
    for_block_definition->condition = expression1;
  } else if (expression3 == NULL) {
    for_block_definition->pre_expression = expression1;
    for_block_definition->condition = expression2;
  } else {
    for_block_definition->pre_expression = expression1;
    for_block_definition->condition = expression2;
    for_block_definition->post_expression = expression3;
  }

  return (BlockDefinition *)for_block_definition;
}

BlockDefinition *parse_block_definition(Lexer *lexer, ParserLimiter limiter) {
  Token token = peek_token(lexer);

  if (token.token_type == IF_TOKEN) {
    return parse_if_block_definition(lexer, limiter);
  } else if (token.token_type == FOR_TOKEN) {
    return parse_for_block_definition(lexer, limiter);
  }

  return NULL;
}

Statement *parse_block_definition_statement(Lexer *lexer, ParserLimiter limiter) {
  BlockDefinitionStatement *block_definition_statement = malloc(sizeof(BlockDefinitionStatement));

  block_definition_statement->statement = (Statement){.statement_type = StatementTypeBlockDefinitionStatement};
  block_definition_statement->block_definition = parse_block_definition(lexer, limiter);

  return (Statement *)block_definition_statement;
}

Statement *parse_print_statement(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  PrintStatement *print_statement = malloc(sizeof(PrintStatement));
  print_statement->statement = (Statement){.statement_type = StatementTypePrintStatement};
  print_statement->right_expression = parse_expression(lexer, 0, limiter);

  return (Statement *)print_statement;
}

Statement *parse_return_statement(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  ReturnStatement *return_statement = malloc(sizeof(ReturnStatement));
  return_statement->statement = (Statement){.statement_type = StatementTypeReturnStatement};
  return_statement->right_expression = parse_expression(lexer, 0, limiter);

  return (Statement *)return_statement;
}

Statement *parse_import_statement(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  ImportStatement *import_statement = malloc(sizeof(ImportStatement));
  import_statement->statement = (Statement){.statement_type = StatementTypeImportStatement};
  import_statement->right_expression = parse_expression(lexer, 0, limiter);

  return (Statement *)import_statement;
}

Statement *parse_expression_statement(Lexer *lexer, ParserLimiter limiter) {
  ExpressionStatement *expression_statement = malloc(sizeof(ExpressionStatement));
  expression_statement->statement = (Statement){.statement_type = StatementTypeExpressionStatement};
  expression_statement->expression = parse_expression(lexer, 0, limiter);

  next_token(lexer);

  return (Statement *)expression_statement;
}

Statement *parse_statement(Lexer *lexer) {
  Token token = peek_token(lexer);

  Statement *statement;

  if (token.token_type == PRINT_TOKEN) {
    statement = parse_print_statement(lexer, DEFAULT_EXPRESSION_PARSER_LIMITER);
  } else if (token.token_type == RETURN_TOKEN) {
    statement = parse_return_statement(lexer, DEFAULT_EXPRESSION_PARSER_LIMITER);
  } else if (token.token_type == IMPORT_TOKEN) {
    statement = parse_import_statement(lexer, DEFAULT_EXPRESSION_PARSER_LIMITER);
  } else if (token.token_type == IF_TOKEN || token.token_type == FOR_TOKEN) {
    statement = parse_block_definition_statement(lexer, DEFAULT_BLOCK_PARSER_LIMITER);
  } else {
    statement = parse_expression_statement(lexer, DEFAULT_EXPRESSION_PARSER_LIMITER);
  }

  return statement;
}

AST *parse_ast(Lexer *lexer) {
  AST *ast = malloc(sizeof(AST));

  ast->block = parse_block(lexer, DEFAULT_BLOCK_PARSER_LIMITER);

  return ast;
}
