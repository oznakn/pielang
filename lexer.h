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
  SEMICOLON_TOKEN,
  IDENTIFIER_TOKEN,
  INTEGER_TOKEN,
  FLOAT_TOKEN,
  BOOL_TOKEN,
  STRING_LITERAL_TOKEN,
  MEMBER_TOKEN,
  COMMA_TOKEN,
  EQUAL_TOKEN,
  PLUS_TOKEN,
  PLUS_EQUAL_TOKEN,
  MINUS_TOKEN,
  MINUS_EQUAL_TOKEN,
  STAR_TOKEN,
  STAR_EQUAL_TOKEN,
  DOUBLE_STAR_TOKEN,
  DOUBLE_STAR_EQUAL_TOKEN,
  SLASH_TOKEN,
  SLASH_EQUAL_TOKEN,
  DOUBLE_SLASH_TOKEN,
  DOUBLE_SLASH_EQUAL_TOKEN,
  CARROT_TOKEN,
  CARROT_EQUAL_TOKEN,
  PERCENTAGE_TOKEN,
  PERCENTAGE_EQUAL_TOKEN,
  PLUS_PLUS_TOKEN,
  MINUS_MINUS_TOKEN,
  L_PARENTHESIS_TOKEN,
  R_PARENTHESIS_TOKEN,
  L_BRACKET_TOKEN,
  R_BRACKET_TOKEN,
  L_BRACE_TOKEN,
  R_BRACE_TOKEN,
  EXCLAMATION_TOKEN,
  DOUBLE_EQUAL_TOKEN,
  EXCLAMATION_EQUAL_TOKEN,
  BIGGER_TOKEN,
  BIGGER_EQUAL_TOKEN,
  SMALLER_TOKEN,
  SMALLER_EQUAL_TOKEN,
  PRINT_TOKEN,
  RETURN_TOKEN,
  IMPORT_TOKEN,
  FUNCTION_TOKEN,
  IF_TOKEN,
  ELSE_TOKEN,
  FOR_TOKEN,
  IN_TOKEN,
} TokenType;

typedef struct {
  long int integer_value;
  double float_value;
  char *string_value;
  bool bool_value;
} Value;

typedef struct {
  TokenType token_type;
  Value value;
} Token;

typedef struct {
  Token curr_token;
  Token next_token;
  FILE *file;
  char curr_char;
  char next_char;
} Lexer;

Lexer *new_lexer(FILE *file);

void free_lexer(Lexer *lexer);

Token parse_string_literal_token(Lexer *lexer, char c);

Token _next_token(Lexer *lexer);

Token next_token(Lexer *lexer);

Token peek_token(Lexer *lexer);

#endif //PIELANG_LEXER_H
