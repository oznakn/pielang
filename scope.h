#ifndef PIELANG_SCOPE_H
#define PIELANG_SCOPE_H

#include <stdlib.h>

#include "hashtable.h"
#include "bool.h"
#include "ast.h"
#include "value.h"

struct Value;
struct Variable;

typedef enum {
  ScopeTypeNormalScope = 1,
  ScopeTypeFunctionScope
} ScopeType;

typedef struct Scope {
  struct Scope *inherited_scope;
  HashTable **variable_maps;
  Block *block;
  ScopeType scope_type;
  struct Value *return_value;
} Scope;


Scope *new_scope(Scope *inherited_scope, Block *block, ScopeType scope_type);


void free_scope(Scope *scope);


void scope_set_return_value(Scope *scope, struct Value *value);


struct Variable *scope_get_variable(Scope *scope, ValueType context_value_type, char *name);


struct Variable *scope_set_variable(Scope *scope, ValueType context_value_type, char *name, struct Value *value, int create_new_if_even_exists) ;


#endif //PIELANG_SCOPE_H
