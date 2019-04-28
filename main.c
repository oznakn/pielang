#include <stdio.h>

#include "bool.h"
#include "lexer.h"
#include "ast.h"
#include "evaluator.h"

#define TEST_MODE true

/* void run_repl() {
  size_t buffer_size = 500000;

  char *buffer = malloc(buffer_size);

  FILE *file;

  Lexer *lexer = NULL;

  Scope *scope = new_scope(NULL, NULL);

  while (true) {
    printf(">> ");

    getline(&buffer, &buffer_size, stdin);

    file = fmemopen(buffer, strlen(buffer), "r");
    if (lexer == NULL) {
      lexer = new_lexer(file);
    }
    else {
      update_lexer(lexer, file);
    }

    Statement *statement = parse_statement(lexer);

    evaluate_statement(scope, statement);

    free_statement(statement);
  }
} */

/*
char *read_file(const char *filename) {
  char *buffer;
  FILE *file = fopen(filename, "r");

  if (file) {
    fseek(file, 0, SEEK_END);

    size_t string_size = (size_t)ftell(file);

    rewind(file);

    buffer = malloc(sizeof(char) * (string_size + 1));

    size_t read_size = fread(buffer, sizeof(char), string_size, file);

    if (string_size != read_size) {
      free(buffer);
      return NULL;
    }

    buffer[string_size] = '\0';

    return buffer;
  }

  return NULL;
}
*/

void run(FILE *file) {
  Lexer *lexer = new_lexer(file);
  AST *ast = parse_ast(lexer);

#if TEST_MODE
  //printf_ast(ast);
  //printf("\n");
#endif

  evaluate_ast(ast);

  free_ast(ast);
  free_lexer(lexer);
}

int main(int argc, char **argv) {
#if TEST_MODE
  FILE *file = fopen("../main.pie", "r");
  if (file) {
    run(file);
  }
#else
  if (argc == 2) {
    char *filename = argv[1];

    FILE *file = fopen(filename, "r");

    if (file) {
      run(file);
    }
  }
  else {
    FILE *file = fopen("./main.pie", "r");
    if (file) {
      run(file);
    }
  }
#endif

  return 0;
}
