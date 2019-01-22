#ifndef PIELANG_PARSER_H
#define PIELANG_PARSER_H

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"
#include "ast.h"

typedef enum {
  DEFAULT_BLOCK_PARSER_LIMITER = 1,
  DEFAULT_EXPRESSION_PARSER_LIMITER,
  GROUPED_EXPRESSION_PARSER_LIMITER,
  TUPLE_EXPRESSION_PARSER_LIMITER,
  DEFAULT_BLOCK_EXPRESSION_PARSER_LIMITER,
  IF_BLOCK_EXPRESSION_PARSER_LIMITER,
  FOR_BLOCK_EXPRESSION_PARSER_LIMITER,
} ParserLimiter;

bool has_finished(Token token, ParserLimiter limiter);

Expression *parse_grouped_expression(Lexer *lexer);

Expression *parse_prefix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter);

Expression *parse_postfix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left);

Expression *parse_tuple_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left);

Expression *parse_call_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left);

Expression *parse_infix_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter, Expression *left);

Expression *parse_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter);

Block *parse_block(Lexer *lexer, ParserLimiter limiter);

BlockDefinition *parse_if_block_definition(Lexer *lexer, ParserLimiter limiter);

BlockDefinition *parse_block_definition(Lexer *lexer, ParserLimiter limiter);

Statement *parse_block_definition_statement(Lexer *lexer, ParserLimiter limiter);

Statement *parse_print_statement(Lexer *lexer, ParserLimiter limiter);

Statement *parse_return_statement(Lexer *lexer, ParserLimiter limiter);

Statement *parse_import_statement(Lexer *lexer, ParserLimiter limiter);

Statement *parse_expression_statement(Lexer *lexer, ParserLimiter limiter);

Statement *parse_statement(Lexer *lexer);

AST *parse_ast(Lexer *lexer);

#endif //PIELANG_PARSER_H
