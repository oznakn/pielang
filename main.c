#define TEST_MODE false

#include <stdio.h>
#include <sys/time.h>

#include "bool.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"

void run_repl() {
  size_t buffer_size = 500000;
  char *buffer = malloc(buffer_size);
  FILE *file;

  Lexer *lexer = NULL;

  while (true) {
    printf(">>> ");

    getline(&buffer, &buffer_size, stdin);

    file = fmemopen(buffer, strlen(buffer), "r");
    if (lexer == NULL) {
      lexer = new_lexer(file);
    } else {
      update_lexer(lexer, file);
    }

    Statement *statement = parse_statement(lexer);

    printf_statement(statement, 0);
    free_statement(statement);
  }
}

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
  struct timeval start, end;
  gettimeofday(&start, NULL);

  Lexer *lexer = new_lexer(file);
  AST *ast = parse_ast(lexer);

  printf_ast(ast);

  free_ast(ast);
  free_lexer(lexer);

  gettimeofday(&end, NULL);

  double delta_us = (end.tv_sec - start.tv_sec) * 1000.0;
  delta_us += (end.tv_usec - start.tv_usec) / 1000.0;

  printf("\nTime elapsed: %fms\n", delta_us);
}

int main(int argc, char **argv) {
  #if TEST_MODE

  FILE *file = fopen("../index.pie", "r");
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
  } else {
    run_repl();
  }
  #endif


  return 0;
}
