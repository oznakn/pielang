#ifndef PIELANG_PARSER_H
#define PIELANG_PARSER_H

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"
#include "ast.h"

Expression *parse_grouped_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2);

Expression *parse_prefix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2);

Expression *parse_postfix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left);

Expression *parse_tuple_expression(Lexer *lexer);

Expression *parse_call_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left);

Expression *parse_infix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left);

Expression *parse_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2);

AST *parse_ast(Lexer *lexer);

#endif //PIELANG_PARSER_H
