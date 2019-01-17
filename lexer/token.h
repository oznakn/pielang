#ifndef PIELANG_TOKEN_H
#define PIELANG_TOKEN_H

#include <stdio.h>

typedef enum {
  NULL_TOKEN = 0,
  EOL,
  IDENTIFIER,
  NUMBER,
  ASSIGN,
  CHECK_EQUALITY,
  STRING_LITERAL,
  ADDITION,
  ASSIGN_ADDITION,
  SUBTRACTION,
  ASSIGN_SUBTRACTION,
  MULTIPLICATION,
  ASSIGN_MULTIPLICATION,
  DIVISION,
  ASSIGN_DIVISION,
  INTEGER_DIVISION,
  ASSIGN_INTEGER_DIVISION,
  FUNCTION_LITERAL,
  IF_LITERAL,
  ELSE_LITERAL,
  FOR_LITERAL,
  L_PARENTHESIS,
  R_PARENTHESIS,
  L_BRACKET,
  R_BRACKET,
  L_BRACE,
  R_BRACE,
} TokenType;

typedef struct {
  TokenType token_type;
  char *content;
} Token;

void print_token(Token token) {
  if (token.token_type == NULL_TOKEN) {
    printf("NULL\n");
  }
  else if (token.content == NULL) {
    printf("%d\n", token.token_type);
  }
  else {
    printf("%d %s\n", token.token_type, token.content);
  }
}

#endif //PIELANG_TOKEN_H
