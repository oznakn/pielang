#ifndef PIELANG_AST_H
#define PIELANG_AST_H

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"

typedef enum {
  StatementTypeExpressionStatement = 1,
  StatementTypePrintStatement,
} StatementType;

typedef enum {
  ExpressionTypeNullExpression = 0,
  ExpressionTypeNumberExpression = 1,
  ExpressionTypeStringExpression,
  ExpressionTypeInfixExpression,
  ExpressionTypePrefixExpression,
  ExpressionTypePostfixExpression,
} ExpressionType;

typedef struct {
  StatementType statement_type;
} Statement;

typedef struct { ;
  float number_value;
  char *string_value;
} Value;

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
  Statement statement;
  Expression *right_expression;
} PrintStatement;

typedef struct {
  Statement **statements;
  size_t statement_count;
} AST;

void print_expression(Expression *expression);

void free_expression(Expression *expression);

void free_statement(Statement *statement);

void free_ast(AST *ast);

AST *parse(Lexer *lexer);

#endif //PIELANG_AST_H
