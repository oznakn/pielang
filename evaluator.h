#ifndef PIELANG_EVALUATOR_H
#define PIELANG_EVALUATOR_H

#include "bool.h"
#include "ast.h"
#include "scope.h"
#include "value.h"

Value *evaluate_infix_expression(Scope *scope, InfixExpression *infix_expression);


Value *evaluate_expression(Scope *scope, Expression *expression);


bool evaluate_print_statement(Scope *scope, PrintStatement *print_statement);


bool evaluate_statement(Scope *scope, Statement *statement);


void evaluate_scope(Scope *scope);


void evaluate_ast(AST *ast);


#endif //PIELANG_EVALUATOR_H
