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
  scope->variable_maps = calloc(VALUE_TYPE_COUNT, sizeof(HashTable *));
  scope->variable_maps[0] = new_hash_table(SCOPE_VARIABLE_HASHTABLE_SIZE, HashTableTypeVariableMap);
  scope->scope_type = scope_type;
  scope->return_value = new_null_value();

  return scope;
}

HashTable *_get_or_create_variable_map(Scope *scope, ValueType context_value_type) {
  if (scope->variable_maps[context_value_type] == NULL) {
    scope->variable_maps[context_value_type] = new_hash_table(SCOPE_VARIABLE_HASHTABLE_SIZE, HashTableTypeVariableMap);
  }

  return scope->variable_maps[context_value_type];
}

void free_scope(Scope *scope) {
  for (size_t i = 0; i < VALUE_TYPE_COUNT; i++) {
    if (scope->variable_maps[i] != NULL) {
      free_hash_table(scope->variable_maps[i]);
    }
  }
  // free(scope->variable_maps);
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


Variable *scope_get_variable(Scope *scope, ValueType context_value_type, char *name) {
  Variable *variable = NULL;

  if (scope->variable_maps[context_value_type] != NULL) {
    variable = variable_map_get(scope->variable_maps[context_value_type], name);
  }

  if (variable != NULL) return variable;

  if (scope->inherited_scope != NULL) {
    variable = scope_get_variable(scope->inherited_scope, context_value_type, name);

    if (variable != NULL) return variable;
  }

  return NULL;
}


Variable *scope_set_variable(Scope *scope, ValueType context_value_type, char *name, Value *value, int create_new_if_even_exists) {
  Variable *variable = scope_get_variable(scope, context_value_type, name);

  if (variable == NULL || create_new_if_even_exists) {
    variable = new_variable(copy_string(name), value);

    HashTable *variable_map = _get_or_create_variable_map(scope, context_value_type);

    variable_map_set(variable_map, variable);

    return variable;
  }
  else {
    if (!variable->is_readonly)  {
      Value *old_value = variable->value;
      old_value->linked_variable_count--;
      free_value(old_value);

      variable->value = value;
      variable->value->linked_variable_count++;
    }
    else {
      // TODO error
    }
  }

  return NULL;
}
