#include "scope.h"

#include <stdlib.h>
#include "hashtable.h"

#define SCOPE_VARIABLE_HASHTABLE_SIZE 20


Scope *new_scope(Scope *inherited_scope, Block *block) {
  Scope *scope = malloc(sizeof(Scope));

  scope->inherited_scope = inherited_scope;
  scope->block = block;
  scope->variable_map = new_hash_table(SCOPE_VARIABLE_HASHTABLE_SIZE, HashTableTypeVariableMap);

  return scope;
}


void free_scope(Scope *scope) {
  free_hash_table(scope->variable_map);
  free(scope);
}


Variable *get_variable(Scope *scope, char *name) {
  return variable_map_get(scope->variable_map, name);
}


void set_variable(Scope *scope, char *name, Value *value) {
  Variable *variable = variable_map_get(scope->variable_map, name);

  if (variable == NULL) {
    variable = new_variable(name, value);

    variable_map_set(scope->variable_map, variable);
  }
  else {
    if (variable->is_readonly) {
      // todo throw error
    }
    else {
      variable->value->linked_variable_count--;

      variable->value = value;
      variable->value->linked_variable_count++;
    }
  }
}