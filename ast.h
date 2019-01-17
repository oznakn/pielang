#ifndef PIELANG_AST_H
#define PIELANG_AST_H

#include "lexer.h"
#include "statement.h"

typedef struct {
  Statement *statements;
} AST;

ExpressionStatement parse_expression_statement(Lexer *lexer) {
  return (ExpressionStatement){};
}

AST *parse(Lexer *lexer) {
  while (true) {
    Token token = next_token(lexer);

    if (token.token_type == NULL_TOKEN) break;

    if (token.token_type == LET_TOKEN) {
      Token identifier = next_token(lexer);

      // TODO

    }
  }
}


#endif //PIELANG_AST_H
