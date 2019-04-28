#include "stringutils.h"

#include <string.h>
#include <stdlib.h>


char *create_string_from_buffer(char *buffer, size_t buffer_length) {
  char *s = calloc(buffer_length + 1, sizeof(char));

  for (size_t i = 0; i < buffer_length; i++) {
    s[i] = buffer[i];
  }

  return s;
}

