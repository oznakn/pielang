#include "lexer.h"

#include <string.h>
#include "bool.h"

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


char _next_char(Lexer *lexer) {
  return (char)fgetc(lexer->file);
}


char peek_char(Lexer *lexer) {
  return lexer->next_char;
}


char next_char(Lexer *lexer) {
  lexer->curr_char = lexer->next_char;
  lexer->next_char = _next_char(lexer);

  return lexer->curr_char;
}


Lexer *new_lexer(FILE *file) {
  Lexer *lexer = malloc(sizeof(Lexer));

  lexer->file = file;
  lexer->next_char = _next_char(lexer);
  lexer->next_token = _next_token(lexer);

  return lexer;
}


void update_lexer(Lexer *lexer, FILE *file) {
  lexer->file = file;
  lexer->next_char = _next_char(lexer);
  lexer->next_token = _next_token(lexer);
}


void free_lexer(Lexer *lexer) {
  free(lexer->file);
  free(lexer);
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
    FloatLiteral *float_literal = malloc(sizeof(FloatLiteral));
    float_literal->literal = (Literal){.literal_type = LiteralTypeFloatLiteral};
    float_literal->float_literal = strtod(string, NULL);

    return (Token){.token_type = FLOAT_TOKEN, .literal = (Literal *)float_literal};
  } else {
    IntegerLiteral *integer_literal = malloc(sizeof(IntegerLiteral));
    integer_literal->literal = (Literal){.literal_type = LiteralTypeIntegerLiteral};
    integer_literal->integer_literal = strtol(string, NULL, 10);

    return (Token){.token_type = INTEGER_TOKEN, .literal = (Literal *)integer_literal};
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

  StringLiteral *string_literal = malloc(sizeof(StringLiteral));
  string_literal->literal = (Literal){.literal_type = LiteralTypeStringLiteral};
  string_literal->string_literal = string;
  string_literal->length = strlen(string);

  return (Token){.token_type = STRING_LITERAL_TOKEN, .literal = (Literal *)string_literal};
}


Token _next_token(Lexer *lexer) {
  skip_whitespace(lexer);

  switch (peek_char(lexer)) {
    case EOF:
    case '\0': {
      return (Token){.token_type = EOF_TOKEN};
    }

    case '\n': {
      next_char(lexer);

      while (peek_char(lexer) == '\n') next_char(lexer);

      return (Token){.token_type = EOL_TOKEN};
    }

    case ';': {
      next_char(lexer);

      return (Token){.token_type = SEMICOLON_TOKEN};
    }

    case '\'': {
      next_char(lexer);

      return parse_string_literal_token(lexer, '\'');
    }

      /* case '"': { // TODO
        return parse_string_literal_token(lexer, '"');
      } */

    case '.': {
      next_char(lexer);

      return (Token){.token_type = MEMBER_TOKEN};
    }

    case ',': {
      next_char(lexer);

      return (Token){.token_type = COMMA_TOKEN};
    }

    case '=': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = DOUBLE_EQUAL_TOKEN};
      }

      return (Token){.token_type = EQUAL_TOKEN};
    }

    case '+': {
      next_char(lexer);

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
      next_char(lexer);

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
      next_char(lexer);

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
      next_char(lexer);

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
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = PERCENTAGE_EQUAL_TOKEN};
      }

      return (Token){.token_type = PERCENTAGE_TOKEN};
    }

    case '^': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = CARROT_EQUAL_TOKEN};
      }

      return (Token){.token_type = CARROT_TOKEN};
    }

    case '!': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = EXCLAMATION_EQUAL_TOKEN};
      }

      return (Token){.token_type = EXCLAMATION_TOKEN};
    }

    case '<': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = SMALLER_EQUAL_TOKEN};
      }

      return (Token){.token_type = SMALLER_TOKEN};
    }

    case '>': {
      next_char(lexer);

      if (peek_char(lexer) == '=') {
        next_char(lexer);

        return (Token){.token_type = BIGGER_EQUAL_TOKEN};
      }

      return (Token){.token_type = BIGGER_TOKEN};
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
        return read_number_token(lexer);
      }

      char *s = read_literal(lexer);

      if (s == NULL) return (Token){.token_type = NULL_TOKEN};

      if (strcmp(s, "true") == 0) {
        free(s);

        // TODO
        BoolLiteral *bool_literal = malloc(sizeof(BoolLiteral));
        bool_literal->literal = (Literal){.literal_type = LiteralTypeBoolLiteral};
        bool_literal->bool_literal = true;

        return (Token){.token_type = BOOL_TOKEN, .literal = (Literal *)bool_literal};
      }
      if (strcmp(s, "false") == 0) {
        free(s);

        // TODO
        BoolLiteral *bool_literal = malloc(sizeof(BoolLiteral));
        bool_literal->literal = (Literal){.literal_type = LiteralTypeBoolLiteral};
        bool_literal->bool_literal = false;

        return (Token){.token_type = BOOL_TOKEN, .literal = (Literal *)bool_literal};
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
      if (strcmp(s, "fun") == 0) {
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
      if (strcmp(s, "in") == 0) {
        free(s);
        return (Token){.token_type = IN_TOKEN};
      }
      if (strcmp(s, "async") == 0) {
        free(s);
        return (Token){.token_type = ASYNC_TOKEN};
      }
      if (strcmp(s, "await") == 0) {
        free(s);
        return (Token){.token_type = AWAIT_TOKEN};
      }

      StringLiteral *string_literal = malloc(sizeof(StringLiteral));
      string_literal->literal = (Literal){.literal_type = LiteralTypeStringLiteral};
      string_literal->string_literal = s;
      string_literal->length = strlen(s);

      return (Token){.token_type = IDENTIFIER_TOKEN, .literal = (Literal *)string_literal};
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
