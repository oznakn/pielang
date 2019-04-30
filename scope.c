#include "scope.h"

#include <stdlib.h>
#include "hashtable.h"
#include "utils.h"
#include "value.h"

#define SCOPE_VARIABLE_HASHTABLE_SIZE 20


Scope *new_scope(Scope *inherited_scope, Block *block, ScopeType scope_type) {
  Scope *scope = malloc(sizeof(Scope));

  scope->inherited_scope = inherited_scope;
  scope->block = block;
  scope->variable_map = new_hash_table(SCOPE_VARIABLE_HASHTABLE_SIZE, HashTableTypeVariableMap);
  scope->scope_type = scope_type;
  scope->return_value = new_null_value();

  return scope;
}


void free_scope(Scope *scope) {
  free_hash_table(scope->variable_map);
  if (scope->block != NULL) free_block(scope->block);
  free(scope);
}


void scope_set_return_value(Scope *scope, Value *value) {
  if (scope->scope_type == ScopeTypeFunctionScope) {
    scope->return_value = value;
  }
  else if (scope->inherited_scope != NULL) {
    scope_set_return_value(scope->inherited_scope, value);
  }
  else {
    // TODO error, no function but return exists
  }
}


Variable *scope_get_variable(Scope *scope, char *name) {
  Variable *variable = variable_map_get(scope->variable_map, name);

  if (variable != NULL) return variable;

  if (scope->inherited_scope != NULL) {
    variable = scope_get_variable(scope->inherited_scope, name);

    if (variable != NULL) return variable;
  }

  return NULL;
}


Variable *scope_set_variable(Scope *scope, char *name, Value *value) {
  Variable *variable = scope_get_variable(scope, name);

  if (variable != NULL) {
    if (!variable->is_readonly)  {
      Value *old_value = variable->value;
      old_value->linked_variable_count--;

      variable->value = value;
      variable->value->linked_variable_count++;
    }
    else {
      // TODO error
    }
  }
  else {
    variable = new_variable(copy_string(name), value);

    variable_map_set(scope->variable_map, variable);

    return variable;
  }

  return NULL;
}
