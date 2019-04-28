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

Scope *new_scope(Scope *inherited_scope, Block *block);

void free_scope(Scope *scope);

Variable *get_variable(Scope *scope, char *name);

#endif //PIELANG_SCOPE_H
