#ifndef PIELANG_EVALUATOR_H
#define PIELANG_EVALUATOR_H

#include "bool.h"
#include "ast.h"
#include "scope.h"
#include "value.h"


Value *call_system_function(SystemFunctionValue *system_function_value, TupleValue *parameter_values);


Value *call_function(Scope *scope, FunctionValue *function_value, TupleValue *parameter_values);


Value *evaluate_index_expression(Scope *scope, IndexExpression *index_expression);


Value *evaluate_infix_expression(Scope *scope, InfixExpression *infix_expression);


Value *evaluate_prefix_expression(Scope *scope, PrefixExpression *prefix_expression);


Value *evaluate_expression(Scope *scope, Expression *expression);


bool evaluate_statement(Scope *scope, Statement *statement);


Value *evaluate_scope(Scope *scope);


void evaluate_ast(AST *ast);


#endif //PIELANG_EVALUATOR_H
