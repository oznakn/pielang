#include <stdio.h>
#include <sys/time.h>

#include "lexer.h"
#include "ast.h"
#include "parser.h"

int main() {
  struct timeval start, end;
  gettimeofday(&start, NULL);

  Lexer *lexer = new_lexer("return (4 + 5 == !(3 + 4) )");
  AST *ast = parse_ast(lexer);

  free_lexer(lexer);
  free_ast(ast);

  gettimeofday(&end, NULL);

  double delta_us = (end.tv_sec - start.tv_sec) * 1000.0;
  delta_us += (end.tv_usec - start.tv_usec) / 1000.0;

  printf("\nTime elapsed: %fms\n", delta_us);

  return 0;
}
