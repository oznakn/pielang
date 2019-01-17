#ifndef PIELANG_LEXER_H
#define PIELANG_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "token.h"
#include "utils.h"

typedef struct {
  size_t size;
  size_t cursor;
  char *content;
} Lexer;

Lexer *new_lexer(char *content) {
  Lexer *lexer = malloc(sizeof(Lexer));
  lexer->content = content;
  lexer->size = strlen(content);
  lexer->cursor = 0;
  return lexer;
}

char peek_char(Lexer *lexer) {
  if (lexer->cursor == lexer->size) return '\0';
  return lexer->content[lexer->cursor];
}

char next_char(Lexer *lexer) {
  if (lexer->cursor == lexer->size) return '\0';
  return lexer->content[lexer->cursor++];
}

void skip_whitespace(Lexer *lexer) {
  while (peek_char(lexer) == ' ') next_char(lexer);
}

char *read_number(Lexer *lexer) {
  size_t i = 1;
  char *string = malloc(1);
  char *tmp = NULL;

  if (!is_number_first_char(peek_char(lexer))) {
    return NULL;
  }

  do {
    if (peek_char(lexer) == '_') {
      next_char(lexer);
      continue;
    }

    string[i - 1] = next_char(lexer);
    tmp = realloc(string, ++i);

    if (tmp == NULL) {
      free(string);

      return NULL;
    }

    string = tmp;
  } while (is_number_char(peek_char(lexer)));

  string[i - 1] = '\0';

  return string;
}


char *read_literal(Lexer *lexer) {
  size_t i = 1;
  char *string = malloc(1);
  char *tmp = NULL;

  if (!is_identifier_first_char(peek_char(lexer))) {
    return NULL;
  }

  do {
    string[i - 1] = next_char(lexer);
    tmp = realloc(string, ++i);

    if (tmp == NULL) {
      free(string);
      return NULL;
    }

    string = tmp;
  } while (is_identifier_char(peek_char(lexer)));

  string[i - 1] = '\0';

  return string;
}

Token next_token(Lexer *lexer) {
  skip_whitespace(lexer);

  switch (peek_char(lexer)) {
    case '\n':
    case ';': {
      next_char(lexer);

      while (peek_char(lexer) == '\n') next_char(lexer);

      return (Token){.token_type = EOL};
    }

    case '\'': {
      next_char(lexer);

      size_t i = 1;

      char *string = malloc(i);
      char *tmp = NULL;

      while (true) {
        if (peek_char(lexer) == '\'') {
          next_char(lexer);
          break;
        }
        else if (peek_char(lexer) == '\\') {
          next_char(lexer);

          if (peek_char(lexer) == 'n') {
            string[i - 1] = '\n';
            tmp = realloc(string, ++i);
          }
          else if (peek_char(lexer) == 't') {
            string[i - 1] = '\t';
            tmp = realloc(string, ++i);
          }
          else if (peek_char(lexer) == 'r') {
            string[i - 1] = '\r';
            tmp = realloc(string, ++i);
          }
          else if (peek_char(lexer) == '\'') {
            string[i - 1] = '\'';
            tmp = realloc(string, ++i);
          }

          next_char(lexer);
        }
        else {
          string[i - 1] = next_char(lexer);
          tmp = realloc(string, ++i);
        }

        if (tmp == NULL) {
          free(string);
          return (Token){.token_type = NULL_TOKEN};
        }
        string = tmp;
      }
      string[i - 1] = '\0';

      return (Token){.token_type = STRING_LITERAL, .content=string};
    }

    case '=': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = CHECK_EQUALITY};
      }

      return (Token){.token_type = ASSIGN};
    }

    case '+': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_ADDITION};
      }

      return (Token){.token_type = ADDITION};
    }

    case '-': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_SUBTRACTION};
      }

      return (Token){.token_type = SUBTRACTION};
    }

    case '*': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_MULTIPLICATION};
      }

      return (Token){.token_type = MULTIPLICATION};
    }

    case '/': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_DIVISION};
      }
      else if (peek_char(lexer) == '/') {
        next_char(lexer);

        if (peek_char(lexer) == '=') {
          next_char(lexer);

          return (Token){.token_type = ASSIGN_INTEGER_DIVISION};
        }

        return (Token){.token_type = INTEGER_DIVISION};
      }

      return (Token){.token_type = DIVISION};
    }

    case '(': {
      next_char(lexer);

      return (Token){.token_type = L_PARENTHESIS};
    }

    case ')': {
      next_char(lexer);

      return (Token){.token_type = R_PARENTHESIS};
    }

    case '[': {
      next_char(lexer);

      return (Token){.token_type = L_BRACKET};
    }

    case ']': {
      next_char(lexer);

      return (Token){.token_type = R_BRACKET};
    }

    case '{': {
      next_char(lexer);

      return (Token){.token_type = L_BRACE};
    }

    case '}': {
      next_char(lexer);

      return (Token){.token_type = R_BRACE};
    }

    default: {
      if (is_digit(peek_char(lexer))) {
        return (Token){.token_type = NUMBER, .content = read_number(lexer)};
      }

      char *s = read_literal(lexer);

      if (s == NULL) return (Token){.token_type = NULL_TOKEN};

      if (strcmp(s, "function") == 0) {
        free(s);
        return (Token){.token_type = FUNCTION_LITERAL};
      }
      if (strcmp(s, "if") == 0) {
        free(s);
        return (Token){.token_type = IF_LITERAL};
      }
      if (strcmp(s, "else") == 0) {
        free(s);
        return (Token){.token_type = ELSE_LITERAL};
      }
      if (strcmp(s, "for") == 0) {
        free(s);
        return (Token){.token_type = FOR_LITERAL};
      }

      return (Token){.token_type = IDENTIFIER, .content = s};
    }
  }
}

#endif //PIELANG_LEXER_H
