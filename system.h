#ifndef PIELANG_SYSTEM_H
#define PIELANG_SYSTEM_H

#include <string.h>
#include "value.h"

Value *apply_prefix_plus_operation(Value *right_value);


Value *apply_prefix_minus_operation(Value *right_value);


Value *apply_addition_operation(Value *left_value, Value *right_value);


Value *apply_subtraction_operation(Value *left_value, Value *right_value);


Value *apply_multiplication_operation(Value *left_value, Value *right_value);


Value *apply_division_operation(Value *left_value, Value *right_value);


Value *apply_exponent_operation(Value *left_value, Value *right_value);


Value *apply_mod_operation(Value *left_value, Value *right_value);


#endif //PIELANG_SYSTEM_H
