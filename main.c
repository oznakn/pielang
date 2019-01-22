#include <stdio.h>
#include <sys/time.h>

#include "lexer.h"
#include "ast.h"
#include "parser.h"

void run_repl() {
  size_t buffer_size = 5000000;
  char *buffer = malloc(buffer_size);

  while (true) {
    printf(">>> ");

    getline(&buffer, &buffer_size, stdin);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    Lexer *lexer = new_lexer(buffer);
    Statement *statement = parse_statement(lexer);
    printf_statement(statement, 0);
    free_statement(statement);
    free_lexer(lexer);

    gettimeofday(&end, NULL);
    double delta_us = (end.tv_sec - start.tv_sec) * 1000.0;
    delta_us += (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Time elapsed: %fms\n", delta_us);
  }

  printf("Exiting...");
}

int main() {

  // run_repl();

  struct timeval start, end;
  gettimeofday(&start, NULL);

  Lexer *lexer = new_lexer("print 'hey'\n\n return 'heyo'\nif a != 3 { print 'merhabalar' }");

  AST *ast = parse_ast(lexer);

  printf_ast(ast);

  free_ast(ast);
  free_lexer(lexer);

  gettimeofday(&end, NULL);

  double delta_us = (end.tv_sec - start.tv_sec) * 1000.0;
  delta_us += (end.tv_usec - start.tv_usec) / 1000.0;

  printf("\nTime elapsed: %fms\n", delta_us);

  return 0;
}
