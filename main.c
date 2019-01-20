#include <stdio.h>

#include "lexer.h"
#include "ast.h"
#include "parser.h"

int main() {
  Lexer *lexer = new_lexer("12 - system.main.call(23) ^ 2 * 3");

  AST *ast = parse_ast(lexer);

  free_lexer(lexer);
  free_ast(ast);

  return 0;
}
