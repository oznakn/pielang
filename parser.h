#ifndef PIELANG_PARSER_H
#define PIELANG_PARSER_H

#include "bool.h"
#include "lexer.h"
#include "ast.h"

typedef enum {
  DEFAULT_BLOCK_PARSER_LIMITER = 1,
  DEFAULT_EXPRESSION_PARSER_LIMITER,
  GROUPED_EXPRESSION_PARSER_LIMITER,
  ARRAY_EXPRESSION_PARSER_LIMITER,
  IF_BLOCK_EXPRESSION_PARSER_LIMITER,
  FOR_BLOCK_EXPRESSION_PARSER_LIMITER,
} ParserLimiter;


bool has_finished(Token token, ParserLimiter limiter);


void *parser_error();


Expression *eval_token(Token token);


Expression *force_tuple(Expression *expression);


Expression *parse_grouped_expression(Lexer *lexer);


Expression *parse_list_expression(Lexer *lexer);


Expression *parse_prefix_expression(Lexer *lexer, ParserLimiter limiter);


Expression *parse_postfix_expression(Lexer *lexer, ParserLimiter limiter, Expression *left);


Expression *parse_array_expression(Lexer *lexer, Expression *left);


Expression *parse_member_expression(Lexer *lexer, Expression *left);


Expression *parse_call_expression(Lexer *lexer, Expression *identifier);


Expression *parse_infix_expression(Lexer *lexer, ParserLimiter limiter, Expression *left);


Expression *parse_function_expression(Lexer *lexer);


Expression *parse_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter);


Block *parse_block(Lexer *lexer, ParserLimiter limiter);


BlockDefinition *parse_if_block_definition(Lexer *lexer, ParserLimiter limiter);


BlockDefinition *parse_for_block_definition(Lexer *lexer, ParserLimiter limiter);


BlockDefinition *parse_block_definition(Lexer *lexer, ParserLimiter limiter);


Statement *parse_block_definition_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_print_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_return_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_import_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_expression_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_statement(Lexer *lexer);


AST *parse_ast(Lexer *lexer);


#endif //PIELANG_PARSER_H
