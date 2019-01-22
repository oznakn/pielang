#include "lexer.h"

#include <string.h>

bool is_digit(char c) {
  return ('0' <= c && c <= '9');
}

bool is_number_char(char c) {
  return is_digit(c) || c == '_';
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

Lexer *new_lexer(char *content) {
  Lexer *lexer = malloc(sizeof(Lexer));

  lexer->content = content;
  lexer->size = strlen(content);
  lexer->cursor = 0;
  lexer->checkpoint = 0;

  lexer->next_token = _next_token(lexer);

  return lexer;
}

void free_lexer(Lexer *lexer) {
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
  while (peek_char(lexer) == ' ' || peek_char(lexer) == '\r' || peek_char(lexer) == '\t') next_char(lexer);
}

Token read_number_token(Lexer *lexer) {
  bool is_float = false;

  size_t i = 1;

  char *string = malloc(1);

  if (!is_digit(peek_char(lexer))) {
    return (Token){.token_type = NULL_TOKEN};
  }

  do {
    if (peek_char(lexer) == '_') {
      next_char(lexer);
      continue;
    } else if (peek_char(lexer) == '.') {
      is_float = true;
    }

    string[i - 1] = next_char(lexer);
    char *tmp = realloc(string, ++i);

    if (tmp == NULL) {
      free(string);

      return (Token){.token_type = NULL_TOKEN};
    }

    string = tmp;
  } while (is_number_char(peek_char(lexer)));

  string[i - 1] = '\0';


  if (is_float) {
    return (Token){.token_type = FLOAT_TOKEN, .value.float_value = strtod(string, NULL)};
  } else {
    return (Token){.token_type = INTEGER_TOKEN, .value.integer_value = strtol(string, NULL, 10)};
  }
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

Token parse_string_literal_token(Lexer *lexer, char c) {
  size_t i = 1;

  char *string = malloc(i);
  char *tmp = NULL;

  while (true) {
    if (peek_char(lexer) == c) {
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
      } else if (peek_char(lexer) == c) {
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

  return (Token){.token_type = STRING_LITERAL_TOKEN, .value.string_value=string};
}

Token _next_token(Lexer *lexer) {
  skip_whitespace(lexer);
  save_checkpoint(lexer);

  switch (next_char(lexer)) {
    case '\0': {
      return (Token){.token_type = EOF_TOKEN};
    }

    case '\n': {
      while (peek_char(lexer) == '\n') next_char(lexer);

      return (Token){.token_type = EOL_TOKEN};
    }

    case ';': {
      return (Token){.token_type = SEMICOLON_TOKEN};
    }

    case '\'': {
      return parse_string_literal_token(lexer, '\'');
    }

      /* case '"': { // TODO
        return parse_string_literal_token(lexer, '"');
      } */

    case '.': {
      return (Token){.token_type = MEMBER_TOKEN};
    }

    case ',': {
      return (Token){.token_type = COMMA_TOKEN};
    }

    case '=': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = DOUBLE_EQUAL_TOKEN};
      }

      return (Token){.token_type = EQUAL_TOKEN};
    }

    case '+': {
      if (peek_char(lexer) == '+') {
        next_char(lexer);

        return (Token){.token_type = PLUS_PLUS_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = PLUS_EQUAL_TOKEN};
      }

      return (Token){.token_type = PLUS_TOKEN};
    }

    case '-': {
      if (peek_char(lexer) == '-') {
        next_char(lexer);

        return (Token){.token_type = MINUS_MINUS_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = MINUS_EQUAL_TOKEN};
      }

      return (Token){.token_type = MINUS_TOKEN};
    }

    case '*': {
      if (peek_char(lexer) == '*') {
        next_char(lexer);

        if (peek_char(lexer) == '=') {
          next_char(lexer);

          return (Token){.token_type = DOUBLE_STAR_EQUAL_TOKEN};
        }

        return (Token){.token_type = DOUBLE_STAR_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = STAR_EQUAL_TOKEN};
      }

      return (Token){.token_type = STAR_TOKEN};
    }

    case '/': {
      if (peek_char(lexer) == '/') {
        next_char(lexer);

        if (peek_char(lexer) == '=') {
          next_char(lexer);

          return (Token){.token_type = DOUBLE_SLASH_EQUAL_TOKEN};
        }

        return (Token){.token_type = DOUBLE_SLASH_TOKEN};
      } else if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = SLASH_EQUAL_TOKEN};
      }

      return (Token){.token_type = SLASH_TOKEN};
    }

    case '%': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = PERCENTAGE_EQUAL_TOKEN};
      }

      return (Token){.token_type = PERCENTAGE_TOKEN};
    }

    case '^': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = CARROT_EQUAL_TOKEN};
      }

      return (Token){.token_type = CARROT_TOKEN};
    }

    case '!': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = EXCLAMATION_EQUAL_TOKEN};
      }

      return (Token){.token_type = EXCLAMATION_TOKEN};
    }

    case '<': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = SMALLER_EQUAL_TOKEN};
      }

      return (Token){.token_type = SMALLER_TOKEN};
    }

    case '>': {
      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = BIGGER_EQUAL_TOKEN};
      }

      return (Token){.token_type = BIGGER_TOKEN};
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
        return read_number_token(lexer);
      }

      char *s = read_literal(lexer);

      if (s == NULL) return (Token){.token_type = NULL_TOKEN};

      if (strcmp(s, "true") == 0) {
        free(s);
        return (Token){.token_type = BOOL_TOKEN, .value.bool_value = true};
      }
      if (strcmp(s, "false") == 0) {
        free(s);
        return (Token){.token_type = BOOL_TOKEN, .value.bool_value = false};
      }
      if (strcmp(s, "print") == 0) {
        free(s);
        return (Token){.token_type = PRINT_TOKEN};
      }
      if (strcmp(s, "return") == 0) {
        free(s);
        return (Token){.token_type = RETURN_TOKEN};
      }
      if (strcmp(s, "import") == 0) {
        free(s);
        return (Token){.token_type = IMPORT_TOKEN};
      }
      if (strcmp(s, "function") == 0) {
        free(s);
        return (Token){.token_type = FUNCTION_TOKEN};
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

      return (Token){.token_type = IDENTIFIER_TOKEN, .value.string_value = s};
    }
  }
}

Token next_token(Lexer *lexer) {
  lexer->curr_token = lexer->next_token;
  lexer->next_token = _next_token(lexer);

  return lexer->curr_token;
}

Token peek_token(Lexer *lexer) {
  return lexer->next_token;
}
