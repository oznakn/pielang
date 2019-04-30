#ifndef PIELANG_SYSTEM_H
#define PIELANG_SYSTEM_H

#include <string.h>
#include <stdlib.h>

#include "value.h"
#include "scope.h"


Value *system_function_print(TupleValue *parameter_values);


void build_main_scope(Scope *scope);


#endif //PIELANG_SYSTEM_H
