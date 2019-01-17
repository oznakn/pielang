#ifndef PIELANG_UTILS_H
#define PIELANG_UTILS_H

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

#endif //PIELANG_UTILS_H
