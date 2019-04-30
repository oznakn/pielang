#include "scope.h"

#include <stdlib.h>
#include "hashtable.h"
#include "value.h"

#define SCOPE_VARIABLE_HASHTABLE_SIZE 20


Scope *new_scope(Scope *inherited_scope, Block *block, bool is_function_scope) {
  Scope *scope = malloc(sizeof(Scope));

  scope->inherited_scope = inherited_scope;
  scope->block = block;
  scope->variable_map = new_hash_table(SCOPE_VARIABLE_HASHTABLE_SIZE, HashTableTypeVariableMap);
  scope->scope_type = is_function_scope ? ScopeTypeFunctionScope : ScopeTypeNormalScope;
  scope->return_value = new_null_value();

  return scope;
}


void free_scope(Scope *scope) {
  free_hash_table(scope->variable_map);
  if (scope->block != NULL) free_block(scope->block);
  free(scope);
}


Variable *scope_get_variable(Scope *scope, char *name) {
  Variable *variable = variable_map_get(scope->variable_map, name);

  if (variable != NULL) return variable;

  if (scope->inherited_scope != NULL) {
    variable = variable_map_get(scope->inherited_scope->variable_map, name);

    if (variable != NULL) return variable;
  }

  return NULL;
}


Variable *scope_set_variable(Scope *scope, char *name, Value *value, int create_new, bool create_new_in_scope) {
  Variable *variable;

  if (create_new == 0) {
    variable = variable_map_get(scope->variable_map, name);

    if (variable == NULL) return scope_set_variable(scope, name, value, 1, create_new_in_scope);
    else return scope_set_variable(scope, name, value, -1, create_new_in_scope);
  }
  else if (create_new == -1) {
    variable = variable_map_get(scope->variable_map, name);

    if (variable != NULL) {
      if (variable->is_readonly) {
        // todo throw error
      }
      else {
        variable->value->linked_variable_count--;

        variable->value = value;
        variable->value->linked_variable_count++;
      }
    }

    return variable;
  }
  else if (create_new == 1) {
    variable = new_variable(name, value);

    if (create_new_in_scope) {
      variable_map_set(scope->variable_map, variable);
    }
    else if (scope->inherited_scope != NULL) {
      variable_map_set(scope->inherited_scope->variable_map, variable);
    }

    return variable;
  }

  return NULL;
}
