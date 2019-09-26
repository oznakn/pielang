#ifndef PIELANG_UTILS_H
#define PIELANG_UTILS_H

#include <stdlib.h>
#include <string.h>

size_t normalize_index(long long int index, long long int length);

char *copy_string(char *s);

char *create_string_from_char(char c);

char *create_string_from_buffer(char *buffer, size_t buffer_length);


#endif //PIELANG_UTILS_H
