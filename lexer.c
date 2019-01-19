#include "lexer.h"

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
  } else if (token.content == NULL) {
    printf("%d\n", token.token_type);
  } else {
    printf("%d %s\n", token.token_type, token.content);
  }
}

Operator token_to_operator(Token token) {
  switch (token.token_type) {
    case ASSIGN_TOKEN:
      return ASSIGN_OP;

    case ADDITION_TOKEN:
      return ADDITION_OP;

    case ASSIGN_ADDITION_TOKEN:
      return ASSIGN_ADDITION_OP;

    case SUBTRACTION_TOKEN:
      return SUBTRACTION_OP;

    case ASSIGN_SUBTRACTION_TOKEN:
      return ASSIGN_SUBTRACTION_OP;

    case MULTIPLICATION_TOKEN:
      return MULTIPLICATION_OP;

    case ASSIGN_MULTIPLICATION_TOKEN:
      return ASSIGN_MULTIPLICATION_OP;

    case DIVISION_TOKEN:
      return DIVISION_OP;

    case ASSIGN_DIVISION_TOKEN:
      return ASSIGN_DIVISION_OP;

    case INTEGER_DIVISION_TOKEN:
      return INTEGER_DIVISION_OP;

    case ASSIGN_INTEGER_DIVISION_TOKEN:
      return ASSIGN_INTEGER_DIVISION_OP;

    case MOD_TOKEN:
      return MOD_OP;

    case ASSIGN_MOD_TOKEN:
      return ASSIGN_MOD_OP;

    case EXPONENT_TOKEN:
      return EXPONENT_OP;

    case ASSIGN_EXPONENT_TOKEN:
      return ASSIGN_EXPONENT_OP;

    case PLUS_PLUS_TOKEN:
      return PLUS_PLUS_OP;

    case MINUS_MINUS_TOKEN:
      return MINUS_MINUS_OP;

    default:
      return -1;
  }
}

bool check_if_token_is_operator(Token token) {
  return token_to_operator(token) != -1;
}

Lexer *new_lexer(char *content) {
  Lexer *lexer = malloc(sizeof(Lexer));

  lexer->content = content;
  lexer->size = strlen(content);
  lexer->cursor = 0;
  lexer->checkpoint = 0;

  return lexer;
}

void free_lexer(Lexer *lexer) {
  // free(lexer->content); TODO
  free(lexer);
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

void reset(Lexer *lexer, size_t cursor) {
  lexer->cursor = cursor;
}

size_t save_checkpoint(Lexer *lexer) {
  lexer->checkpoint = lexer->cursor;
  return lexer->cursor;
}

void go_checkpoint(Lexer *lexer) {
  lexer->cursor = lexer->checkpoint;
}

Token peek_token(Lexer *lexer) {
  size_t checkpoint = save_checkpoint(lexer);

  Token token = next_token(lexer);

  reset(lexer, checkpoint);

  return token;
}

Token next_token(Lexer *lexer) {
  skip_whitespace(lexer);
  save_checkpoint(lexer);

  switch (next_char(lexer)) {
    case '\0': {
      return (Token){.token_type = EOF_TOKEN};
    }

    case '\n':
    case ';': {
      while (peek_char(lexer) == '\n') next_char(lexer);

      return (Token){.token_type = EOL_TOKEN};
    }

    case '\'': {
      size_t i = 1;

      char *string = malloc(i);
      char *tmp = NULL;

      while (true) {
        if (peek_char(lexer) == '\'') {
          next_char(lexer);
          break;
        } else if (peek_char(lexer) == '\\') {
          next_char(lexer);

          if (peek_char(lexer) == 'n') {
            string[i - 1] = '\n';
            tmp = realloc(string, ++i);
          } else if (peek_char(lexer) == 't') {
            string[i - 1] = '\t';
            tmp = realloc(string, ++i);
          } else if (peek_char(lexer) == 'r') {
            string[i - 1] = '\r';
            tmp = realloc(string, ++i);
          } else if (peek_char(lexer) == '\'') {
            string[i - 1] = '\'';
            tmp = realloc(string, ++i);
          }

          next_char(lexer);
        } else {
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
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = CHECK_EQUALITY_TOKEN};
      }

      return (Token){.token_type = ASSIGN_TOKEN};
    }

    case '+': {
      if (peek_char(lexer) == '+') {
        next_char(lexer);

        return (Token){.token_type = PLUS_PLUS_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_ADDITION_TOKEN};
      }

      return (Token){.token_type = ADDITION_TOKEN};
    }

    case '-': {
      if (peek_char(lexer) == '-') {
        next_char(lexer);

        return (Token){.token_type = MINUS_MINUS_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_SUBTRACTION_TOKEN};
      }

      return (Token){.token_type = SUBTRACTION_TOKEN};
    }

    case '*': {
      if (peek_char(lexer) == '*') {
        next_char(lexer);

        if (peek_char(lexer) == '=') {
          next_char(lexer);

          return (Token){.token_type = ASSIGN_EXPONENT_TOKEN};
        }

        return (Token){.token_type = EXPONENT_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_MULTIPLICATION_TOKEN};
      }

      return (Token){.token_type = MULTIPLICATION_TOKEN};
    }

    case '/': {
      if (peek_char(lexer) == '/') {
        next_char(lexer);

        if (peek_char(lexer) == '=') {
          next_char(lexer);

          return (Token){.token_type = ASSIGN_INTEGER_DIVISION_TOKEN};
        }

        return (Token){.token_type = INTEGER_DIVISION_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_DIVISION_TOKEN};
      }

      return (Token){.token_type = DIVISION_TOKEN};
    }

    case '%': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_MOD_TOKEN};
      }

      return (Token){.token_type = MOD_TOKEN};
    }

    case '^': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = ASSIGN_EXPONENT_TOKEN};
      }

      return (Token){.token_type = EXPONENT_TOKEN};
    }

    case '(': {
      return (Token){.token_type = L_PARENTHESIS_TOKEN};
    }

    case ')': {
      return (Token){.token_type = R_PARENTHESIS_TOKEN};
    }

    case '[': {
      return (Token){.token_type = L_BRACKET_TOKEN};
    }

    case ']': {
      return (Token){.token_type = R_BRACKET_TOKEN};
    }

    case '{': {
      return (Token){.token_type = L_BRACE_TOKEN};
    }

    case '}': {
      return (Token){.token_type = R_BRACE_TOKEN};
    }

    default: {
      go_checkpoint(lexer);

      if (is_digit(peek_char(lexer))) {
        return (Token){.token_type = NUMBER_TOKEN, .content = read_number(lexer)};
      }

      char *s = read_literal(lexer);

      if (s == NULL) return (Token){.token_type = NULL_TOKEN};

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

