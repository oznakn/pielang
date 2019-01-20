#include <stdio.h>

#include "lexer.h"
#include "ast.h"

int main() {
  Lexer *lexer = new_lexer("a = (2 + -4 * math(w,3))");

  AST *ast = parse(lexer);

  free_lexer(lexer);
  free_ast(ast);

  return 0;
}
