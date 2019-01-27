#ifndef PIELANG_SCOPE_H
#define PIELANG_SCOPE_H

#include <stdlib.h>

#include "bool.h"
#include "ast.h"
#include "value.h"

typedef struct Scope {
  struct Scope *inherited_scope;
  HashTable *variable_map;
  Block *block;
} Scope;

Scope *new_scope(Scope *inherited_scope);

void run_scope(Scope *scope);

void free_scope(Scope *scope);

#endif //PIELANG_SCOPE_H
