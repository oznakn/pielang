#ifndef PIELANG_LEXER_H
#define PIELANG_LEXER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  NULL_TOKEN = 0,
  EOL_TOKEN,
  IDENTIFIER_TOKEN,
  NUMBER_TOKEN,
  ASSIGN_TOKEN,
  CHECK_EQUALITY_TOKEN,
  STRING_LITERAL_TOKEN,
  ADDITION_TOKEN,
  ASSIGN_ADDITION_TOKEN,
  SUBTRACTION_TOKEN,
  ASSIGN_SUBTRACTION_TOKEN,
  MULTIPLICATION_TOKEN,
  ASSIGN_MULTIPLICATION_TOKEN,
  DIVISION_TOKEN,
  ASSIGN_DIVISION_TOKEN,
  INTEGER_DIVISION_TOKEN,
  ASSIGN_INTEGER_DIVISION_TOKEN,
  LET_TOKEN,
  FUNCTION_TOKEN,
  IF_TOKEN,
  ELSE_TOKEN,
  FOR_TOKEN,
  L_PARENTHESIS_TOKEN,
  R_PARENTHESIS_TOKEN,
  L_BRACKET_TOKEN,
  R_BRACKET_TOKEN,
  L_BRACE_TOKEN,
  R_BRACE_TOKEN,
} TokenType;

typedef struct {
  TokenType token_type;
  char *content;
} Token;

typedef struct {
  size_t size;
  size_t cursor;
  char *content;
} Lexer;


bool is_digit(char c) {
  return ('0' <= c && c <= '9');
}

bool is_number_first_char(char c) {
  return is_digit(c) || c == '.';
}

bool is_number_char(char c) {
  return is_number_first_char(c) || c == '_';
}

bool is_alpha(char c) {
  return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

bool is_alphanum(char c) {
  return is_alpha(c) || is_digit(c);
}

bool is_identifier_first_char(char c) {
  return is_alpha(c) || c == '_';
}

bool is_identifier_char(char c) {
  return is_identifier_first_char(c) || is_digit(c);
}

void print_token(Token token) {
  if (token.token_type == NULL_TOKEN) {
    printf("NULL\n");
  }
  else if (token.content == NULL) {
    printf("%d\n", token.token_type);
  }
  else {
    printf("%d %s\n", token.token_type, token.content);
  }
}

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

      return (Token){.token_type = EOL_TOKEN};
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

      return (Token){.token_type = STRING_LITERAL_TOKEN, .content=string};
    }

    case '=': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = CHECK_EQUALITY_TOKEN};
      }

      return (Token){.token_type = ASSIGN_TOKEN};
    }

    case '+': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_ADDITION_TOKEN};
      }

      return (Token){.token_type = ADDITION_TOKEN};
    }

    case '-': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_SUBTRACTION_TOKEN};
      }

      return (Token){.token_type = SUBTRACTION_TOKEN};
    }

    case '*': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_MULTIPLICATION_TOKEN};
      }

      return (Token){.token_type = MULTIPLICATION_TOKEN};
    }

    case '/': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_DIVISION_TOKEN};
      }
      else if (peek_char(lexer) == '/') {
        next_char(lexer);

        if (peek_char(lexer) == '=') {
          next_char(lexer);

          return (Token){.token_type = ASSIGN_INTEGER_DIVISION_TOKEN};
        }

        return (Token){.token_type = INTEGER_DIVISION_TOKEN};
      }

      return (Token){.token_type = DIVISION_TOKEN};
    }

    case '(': {
      next_char(lexer);

      return (Token){.token_type = L_PARENTHESIS_TOKEN};
    }

    case ')': {
      next_char(lexer);

      return (Token){.token_type = R_PARENTHESIS_TOKEN};
    }

    case '[': {
      next_char(lexer);

      return (Token){.token_type = L_BRACKET_TOKEN};
    }

    case ']': {
      next_char(lexer);

      return (Token){.token_type = R_BRACKET_TOKEN};
    }

    case '{': {
      next_char(lexer);

      return (Token){.token_type = L_BRACE_TOKEN};
    }

    case '}': {
      next_char(lexer);

      return (Token){.token_type = R_BRACE_TOKEN};
    }

    default: {
      if (is_digit(peek_char(lexer))) {
        return (Token){.token_type = NUMBER_TOKEN, .content = read_number(lexer)};
      }

      char *s = read_literal(lexer);

      if (s == NULL) return (Token){.token_type = NULL_TOKEN};

      if (strcmp(s, "let") == 0) {
        free(s);
        return (Token){.token_type = LET_TOKEN};
      }
      if (strcmp(s, "function") == 0) {
        free(s);
        return (Token){.token_type = FUNCTION_TOKEN};
      }
      if (strcmp(s, "if") == 0) {
        free(s);
        return (Token){.token_type = IF_TOKEN};
      }
      if (strcmp(s, "else") == 0) {
        free(s);
        return (Token){.token_type = ELSE_TOKEN};
      }
      if (strcmp(s, "for") == 0) {
        free(s);
        return (Token){.token_type = FOR_TOKEN};
      }

      return (Token){.token_type = IDENTIFIER_TOKEN, .content = s};
    }
  }
}

#endif //PIELANG_LEXER_H
