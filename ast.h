#ifndef PIELANG_AST_H
#define PIELANG_AST_H

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"

typedef enum {
  ASSIGN_OP = 1,
  MEMBER_OP,
  ADDITION_OP,
  ASSIGN_ADDITION_OP,
  SUBTRACTION_OP,
  ASSIGN_SUBTRACTION_OP,
  MULTIPLICATION_OP,
  ASSIGN_MULTIPLICATION_OP,
  DIVISION_OP,
  ASSIGN_DIVISION_OP,
  INTEGER_DIVISION_OP,
  ASSIGN_INTEGER_DIVISION_OP,
  EXPONENT_OP,
  ASSIGN_EXPONENT_OP,
  MOD_OP,
  ASSIGN_MOD_OP,
  PLUS_PLUS_OP,
  MINUS_MINUS_OP,
  L_PARENTHESIS_OP,
} Operator;

typedef enum {
  StatementTypeExpressionStatement = 1,
  StatementTypePrintStatement,
} StatementType;

typedef enum {
  ExpressionTypeNullExpression = 0,
  ExpressionTypeIntegerExpression,
  ExpressionTypeFloatExpression,
  ExpressionTypeStringExpression,
  ExpressionTypeBoolExpression,
  ExpressionTypeIdentifierExpression,
  ExpressionTypeInfixExpression,
  ExpressionTypePrefixExpression,
  ExpressionTypePostfixExpression,
  ExpressionTypeCallExpression,
  ExpressionTypeTupleExpression,
} ExpressionType;

typedef struct {
  StatementType statement_type;
} Statement;

typedef struct {
  ExpressionType expression_type;
  Value value;
} Expression;

typedef struct {
  Statement statement;
  Expression *expression;
} ExpressionStatement;

typedef struct {
  Statement statement;
} DefinitionStatement;

typedef struct {
  Expression expression;
  Operator operator;
  Expression *left_expression;
  Expression *right_expression;
} InfixExpression;

typedef struct {
  Expression expression;
  Operator operator;
  Expression *right_expression;
} PrefixExpression;

typedef struct {
  Expression expression;
  Operator operator;
  Expression *left_expression;
} PostfixExpression;

typedef struct {
  Expression expression;
  Expression **expressions;
  size_t expression_count;
} TupleExpression;

typedef struct {
  Expression expression;
  char *identifier;
  TupleExpression *tuple_expression;
} CallExpression;

typedef struct {
  Statement statement;
  Expression *right_expression;
} PrintStatement;

typedef struct {
  Statement **statements;
  size_t statement_count;
} AST;

Operator token_to_operator(Token token);

bool check_if_token_is_operator(Token token);


bool is_right_associative(Token token);

bool check_if_token_is_postfix_operator(Token token);

unsigned short get_operator_precedence(Token token, unsigned short precedence);

void print_expression(Expression *expression);

void free_expression(Expression *expression);

void free_statement(Statement *statement);

void free_ast(AST *ast);

Expression *parse_grouped_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2);

Expression *parse_prefix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2);

Expression *parse_postfix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left);

Expression *parse_infix_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2, Expression *left);

Expression *parse_expression(Lexer *lexer, unsigned short precedence, TokenType until1, TokenType until2);

AST *parse(Lexer *lexer);

#endif //PIELANG_AST_H
