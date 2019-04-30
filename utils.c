#include "utils.h"

#include <string.h>
#include <stdlib.h>

#include "bool.h"
#include "value.h"


char *copy_string(char *s) {
  char *result = calloc(strlen(s) + 1, sizeof(char));
  strcpy(result, s);
  return result;
}


// usage for buffer with no \0 at end ex.
char *create_string_from_buffer(char *buffer, size_t buffer_length) {
  char *s = calloc(buffer_length + 1, sizeof(char));

  for (size_t i = 0; i < buffer_length; i++) {
    s[i] = buffer[i];
  }

  return s;
}
