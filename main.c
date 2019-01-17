#include <stdio.h>

#include "lexer/lexer.h"

int main() {
  Lexer *lexer = new_lexer("let a = 'anan' 100_000.5 124\n_b //= x");

  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));
  print_token(next_token(lexer));


  free(lexer);

  return 0;
}
