#include "system.h"

#include <string.h>
#include <stdlib.h>

#include "value.h"
#include "scope.h"

Value *system_function_print(TupleValue *parameter_values) {
  for (size_t i = 0; i < parameter_values->length; i++) {
    StringValue *string_value = (StringValue *) convert_to_string_value(parameter_values->items[i]);

    printf("%s", string_value->string_value);

    if (i != parameter_values->length - 1) {
      printf(" ");
    }

    free_value((Value *) string_value);
  }

  printf("\n");

  return new_null_value();
}


void _build_system_function(Scope *scope, char *s, Value *value) {
  int length = strlen(s);
  char *name = calloc(length + 1, sizeof(char));

  strcpy(name, s);

  Variable *variable = scope_set_variable(scope, name, value, 1, true);
  variable->is_readonly = true;
}


void build_main_scope(Scope *scope) {
  _build_system_function(scope, "print", new_system_function_value(system_function_print));
}

