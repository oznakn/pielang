#ifndef PIELANG_STATEMENTS_H
#define PIELANG_STATEMENTS_H

#include "lexer.h"

typedef TokenType Operator;

typedef enum {
  StatementTypeExpressionStatement = 1,
  StatementTypePrintStatement,
} StatementType;

typedef enum {
  ExpressionStatementTypeInfixExpression = 1,
  ExpressionStatementTypePrintExpression,
} ExpressionStatementType;


typedef struct {
  int32_t int_value;
  double double_value;
  char *string_value;
} Value;

typedef struct {
  StatementType statement_type;
} Statement;

typedef struct {
  Statement self;
  ExpressionStatementType expression_statement_type;
  Value value;
} ExpressionStatement;

typedef struct {
  ExpressionStatement self;
  ExpressionStatement left_expression_statement;
  Operator operator;
  ExpressionStatement right_expression_statement;
} InfixExpressionStatement;

typedef struct {
  ExpressionStatement self;
  ExpressionStatement expression_statement;
} PrintExpressionStatement;

typedef struct {
  Statement self;
  char *identifier;
  ExpressionStatement expression_statement;
} LetStatement;

#endif //PIELANG_STATEMENTS_H
