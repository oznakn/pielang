#include <stdio.h>

#include "lexer.h"
#include "ast.h"

int main() {
  Lexer *lexer = new_lexer("4++ +5");

  AST *ast = parse(lexer);

  free_lexer(lexer);
  free_ast(ast);

  return 0;
}
