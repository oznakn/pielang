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

  Lexer *lexer = NULL;
  Scope *scope = new_scope(NULL, NULL, ScopeTypeNormalScope);

  build_main_scope(scope);

  while ((s = linenoise(">> ")) != NULL) {
    if (lexer == NULL) {
      lexer = new_lexer(s);
    }
    else {
      update_lexer(lexer, s);
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

char *read_file(const char *filename) {
  char *buffer;
  FILE *file = fopen(filename, "r");

  if (file) {
    fseek(file, 0, SEEK_END);

    size_t string_size = (size_t) ftell(file);

    rewind(file);

    buffer = malloc(sizeof(char) * (string_size + 1));

    size_t read_size = fread(buffer, sizeof(char), string_size, file);

    if (string_size != read_size) {
      free(buffer);
      return NULL;
    }

    buffer[string_size] = 0;

    return buffer;
  }

  return NULL;
}


void run(char *filename) {
  char *s = read_file(filename);

  Lexer *lexer = new_lexer(s);
  AST *ast = parse_ast(lexer);

#if TEST_MODE

  // printf_ast(ast);

#endif

  evaluate_ast(ast);

  free_ast(ast);
  free_lexer(lexer);

  free(s);
}

int main(int argc, char **argv) {
#if TEST_MODE
  run("../main.pie");
#else
  if (argc == 2) {
    run(argv[1]);
  }
  else {
    run_repl();
  }
#endif

  return 0;
}
