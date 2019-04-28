#ifndef PIELANG_EVALUATOR_H
#define PIELANG_EVALUATOR_H

#include "bool.h"
#include "ast.h"
#include "scope.h"
#include "value.h"


Value *apply_prefix_not_operation(Value *right_value);


Value *apply_prefix_plus_operation(Value *right_value);


Value *apply_prefix_minus_operation(Value *right_value);


Value *apply_addition_operation(Value *left_value, Value *right_value);


Value *apply_subtraction_operation(Value *left_value, Value *right_value);


Value *apply_multiplication_operation(Value *left_value, Value *right_value);


Value *apply_division_operation(Value *left_value, Value *right_value);


Value *apply_exponent_operation(Value *left_value, Value *right_value);


Value *apply_mod_operation(Value *left_value, Value *right_value);


Value *apply_assign_operation(Scope *scope, char *identifier, Value *right_value);


Value *evaluate_infix_expression(Scope *scope, InfixExpression *infix_expression);


Value *evaluate_expression(Scope *scope, Expression *expression);


bool evaluate_print_statement(Scope *scope, PrintStatement *print_statement);


bool evaluate_statement(Scope *scope, Statement *statement);


void evaluate_scope(Scope *scope);


void evaluate_ast(AST *ast);


#endif //PIELANG_EVALUATOR_H
