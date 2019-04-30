#include <stdio.h>

#include "bool.h"
#include "lexer.h"
#include "ast.h"
#include "evaluator.h"
#include "system.h"
#include "linenoise.h"

#define TEST_MODE true

void run_repl() {
  char *s;

  FILE *file;

  Lexer *lexer = NULL;
  Scope *scope = new_scope(NULL, NULL, ScopeTypeNormalScope);

  build_main_scope(scope);

  while ((s = linenoise(">> ")) != NULL) {
    file = fmemopen(s, strlen(s), "r");

    if (lexer == NULL) {
      lexer = new_lexer(file);
    }
    else {
      update_lexer(lexer, file);
    }

    Statement *statement = parse_statement(lexer);

    if (statement != NULL) {
      linenoiseHistoryAdd(s);

      evaluate_statement(scope, statement, true);

      free_statement(statement);
    }

    free(s);
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
  Lexer *lexer = new_lexer(file);
  AST *ast = parse_ast(lexer);

#if TEST_MODE

  // printf_ast(ast);

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
    run_repl();
  }
#endif

  return 0;
}
