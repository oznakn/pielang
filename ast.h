#ifndef PIELANG_AST_H
#define PIELANG_AST_H

#include "bool.h"

#define ASSIGN_PRECEDENCE 1
#define COMMA_PRECEDENCE 2
#define ASYNC_AWAIT_PRECEDENCE 3
#define IN_OP_PRECEDENCE 4
#define CONDITIONAL_PRECEDENCE 5
#define ADDITION_SUBTRACTION_PRECEDENCE 6
#define MULTIPLICATION_DIVISION_MOD_PRECEDENCE 7
#define EXPONENT_PRECEDENCE 8
#define PLUS_PLUS_MINUS_MINUS_PRECEDENCE 9
#define CALL_PRECEDENCE 10
#define LIST_PRECEDENCE 11
#define MEMBER_PRECEDENCE 12

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
  L_BRACKET_OP,
  NOT_OP,
  CHECK_EQUALITY_OP,
  CHECK_NOT_EQUALITY_OP,
  CHECK_BIGGER_OP,
  CHECK_BIGGER_EQUAL_OP,
  CHECK_SMALLER_OP,
  CHECK_SMALLER_EQUAL_OP,
  IN_OP,
  COMMA_OP,
  ASYNC_OP,
  AWAIT_OP,
} Operator;

typedef enum {
  StatementTypeExpressionStatement = 1,
  StatementTypeBlockDefinitionStatement,
  StatementTypePrintStatement,
  StatementTypeReturnStatement,
  StatementTypeImportStatement,
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
  ExpressionTypeArrayExpression,
  ExpressionTypeMemberExpression,
  ExpressionTypeFunctionExpression,
} ExpressionType;

typedef enum {
  BlockDefinitionTypeIfBlock = 1,
  BlockDefinitionTypeForBlock,
} BlockDefinitionType;

typedef enum {
  ArrayExpressionTypeList,
  ArrayExpressionTypeTuple,
} ArrayExpressionType;

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
  ArrayExpressionType array_expression_type;
} ArrayExpression;

typedef struct {
  Expression expression;
  Expression **expressions;
  size_t expression_count;
} MemberExpression;

typedef struct {
  Expression expression;
  Expression *identifier_expression;
  Expression *tuple_expression;
} CallExpression;

typedef struct {
  Statement statement;
  Expression *right_expression;
} PrintStatement, ReturnStatement, ImportStatement;

typedef struct {
  Statement **statements;
  size_t statement_count;
} Block;

typedef struct {
  BlockDefinitionType block_definition_type;
} BlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  Block *block;
  Expression *pre_expression;
  Expression *condition;
} IfBlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  Block *block;
  Expression *pre_expression;
  Expression *condition;
  Expression *post_expression;
} ForBlockDefinition;

typedef struct {
  Expression expression;
  Block *block;
  Expression *identifier;
  Expression *arguments;
} FunctionExpression;

typedef struct {
  Statement statement;
  BlockDefinition *block_definition;
} BlockDefinitionStatement;

typedef struct {
  Block *block;
} AST;

void printf_alignment(unsigned int alignment);

void printf_expression(Expression *expression, unsigned int alignment);

void printf_block(Block *block, unsigned int alignment);

void printf_block_definition(BlockDefinition *block_definition, unsigned int alignment);

void printf_statement(Statement *statement, unsigned int alignment);

void printf_ast(AST *ast);

void free_expression(Expression *expression);

void free_statement(Statement *statement);

void free_block(Block *block);

void free_block_definition(BlockDefinition *block_definition);

void free_ast(AST *ast);

Operator token_to_operator(Token token);

bool check_if_token_is_operator(Token token);

bool check_if_token_is_postfix_operator(Token token);

bool is_operator_right_associative(Operator operator);

unsigned short get_operator_precedence(Operator operator, bool next);

#endif //PIELANG_AST_H
