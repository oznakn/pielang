#include <string.h>
#include <stdlib.h>

size_t normalize_index(long long int index, long long int length) {
  if (index >= length) {
    index = index % length;
  }
  else if (index < 0) {
    index = (length + index) % length;
  }

  return index;
}


char *copy_string(char *s) {
  char *result = calloc(strlen(s) + 1, sizeof(char));
  strcpy(result, s);
  return result;
}

char *create_string_from_char(char c) {
  char *s = calloc(2, sizeof(char));

  s[0] = c;

  return s;
}

// usage for buffer with no \0 at end ex.
char *create_string_from_buffer(char *buffer, size_t buffer_length) {
  char *s = calloc(buffer_length + 1, sizeof(char));

  for (size_t i = 0; i < buffer_length; i++) {
    s[i] = buffer[i];
  }

  return s;
}
