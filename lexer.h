#ifndef PIELANG_LEXER_H
#define PIELANG_LEXER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  NULL_TOKEN = 0,
  EOL_TOKEN,
  EOF_TOKEN,
  IDENTIFIER_TOKEN,
  NUMBER_TOKEN,
  STRING_LITERAL_TOKEN,
  ASSIGN_TOKEN,
  CHECK_EQUALITY_TOKEN,
  ADDITION_TOKEN,
  ASSIGN_ADDITION_TOKEN,
  SUBTRACTION_TOKEN,
  ASSIGN_SUBTRACTION_TOKEN,
  MULTIPLICATION_TOKEN,
  ASSIGN_MULTIPLICATION_TOKEN,
  DIVISION_TOKEN,
  ASSIGN_DIVISION_TOKEN,
  INTEGER_DIVISION_TOKEN,
  ASSIGN_INTEGER_DIVISION_TOKEN,
  EXPONENT_TOKEN,
  ASSIGN_EXPONENT_TOKEN,
  MOD_TOKEN,
  ASSIGN_MOD_TOKEN,
  PLUS_PLUS_TOKEN,
  MINUS_MINUS_TOKEN,
  L_PARENTHESIS_TOKEN,
  R_PARENTHESIS_TOKEN,
  L_BRACKET_TOKEN,
  R_BRACKET_TOKEN,
  L_BRACE_TOKEN,
  R_BRACE_TOKEN,
  FUNCTION_TOKEN,
  IF_TOKEN,
  ELSE_TOKEN,
  FOR_TOKEN,
} TokenType;

typedef enum {
  ASSIGN_OP = 1,
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
} Operator;

typedef struct {
  TokenType token_type;
  char *content;
} Token;

typedef struct {
  size_t size;
  size_t cursor;
  size_t checkpoint;
  char *content;
} Lexer;

Operator token_to_operator(Token token);

bool check_if_token_is_operator(Token token);

Lexer *new_lexer(char *content);

void free_lexer(Lexer *lexer);

void print_token(Token token);

void reset(Lexer *lexer, size_t cursor);

size_t save_checkpoint(Lexer *lexer);

void go_checkpoint(Lexer *lexer);

Token peek_token(Lexer *lexer);

Token next_token(Lexer *lexer);

#endif //PIELANG_LEXER_H
