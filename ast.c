#include "ast.h"

#include <string.h>
#include <signal.h>

#include "bool.h"
#include "lexer.h"
#include "utils.h"

#define MAX_BUFFER_SIZE 10000


void printf_token(Token token) {
  switch (token.token_type) {
    case NULL_TOKEN: {
      printf(" null ");

      break;
    }

    case IDENTIFIER_TOKEN: {
      printf(" %s ", ((StringLiteral *)token.literal)->string_literal);

      break;
    }

    case INTEGER_TOKEN: {
      printf(" %lld ", ((IntegerLiteral *)token.literal)->integer_literal);

      break;
    }

    case FLOAT_TOKEN: {
      printf(" %Lf ", ((FloatLiteral *)token.literal)->float_literal);

      break;
    }

    case BOOL_TOKEN: {
      printf(" %s ", ((BoolLiteral *)token.literal)->bool_literal ? "true" : "false");

      break;
    }

    case STRING_LITERAL_TOKEN: {
      printf(" \"%s\" ", ((StringLiteral *)token.literal)->string_literal);

      break;
    }
  }
}


void printf_alignment(unsigned int alignment) {
  if (alignment != 0u) {
    for (unsigned int i = 0; i < alignment; i += 1u) {
      printf("    ");
    }
  }
}


void printf_expression(Expression *expression, unsigned int alignment) {
  switch (expression->expression_type) {
    case ExpressionTypeInfixExpression: {
      InfixExpression *infix_expression = (InfixExpression *) expression;

      printf(" (");
      printf_expression(infix_expression->left_expression, alignment);

      if (infix_expression->operator == ADDITION_OP) { printf(" + "); }
      else if (infix_expression->operator == SUBTRACTION_OP) { printf(" - "); }
      else if (infix_expression->operator == MULTIPLICATION_OP) { printf(" * "); }
      else if (infix_expression->operator == DIVISION_OP) { printf(" / "); }
      else if (infix_expression->operator == MOD_OP) { printf(" %% "); }
      else if (infix_expression->operator == EXPONENT_OP) { printf(" ^ "); }
      else if (infix_expression->operator == ASSIGN_OP) { printf(" = "); }
      else if (infix_expression->operator == ASSIGN_ADDITION_OP) { printf(" += "); }
      else if (infix_expression->operator == ASSIGN_SUBTRACTION_OP) { printf(" -= "); }
      else if (infix_expression->operator == ASSIGN_MULTIPLICATION_OP) { printf(" *= "); }
      else if (infix_expression->operator == ASSIGN_DIVISION_OP) { printf(" /= "); }
      else if (infix_expression->operator == ASSIGN_MOD_OP) { printf(" %%= "); }
      else if (infix_expression->operator == ASSIGN_EXPONENT_OP) { printf(" ^= "); }
      else if (infix_expression->operator == CHECK_EQUALITY_OP) { printf(" == "); }
      else if (infix_expression->operator == CHECK_NOT_EQUALITY_OP) { printf(" != "); }
      else if (infix_expression->operator == CHECK_SMALLER_OP) { printf(" < "); }
      else if (infix_expression->operator == CHECK_SMALLER_EQUAL_OP) { printf(" <= "); }
      else if (infix_expression->operator == CHECK_BIGGER_OP) { printf(" > "); }
      else if (infix_expression->operator == CHECK_BIGGER_EQUAL_OP) { printf(" >= "); }
      else if (infix_expression->operator == IN_OP) { printf(" in "); }
      else if (infix_expression->operator == COMMA_OP) { printf(" , "); }

      printf_expression(infix_expression->right_expression, alignment);
      printf(") ");

      break;
    }

    case ExpressionTypePrefixExpression: {
      PrefixExpression *prefix_expression = (PrefixExpression *) expression;

      printf(" (");

      if (prefix_expression->operator == NOT_OP) { printf("!"); }
      else if (prefix_expression->operator == ADDITION_OP) { printf("+"); }
      else if (prefix_expression->operator == SUBTRACTION_OP) { printf("-"); }
      else if (prefix_expression->operator == ASYNC_OP) { printf(" async "); }
      else if (prefix_expression->operator == AWAIT_OP) { printf(" await "); }

      printf_expression(prefix_expression->right_expression, alignment);
      printf(") ");

      break;
    }


    case ExpressionTypeIndexExpression: {
      IndexExpression *index_expression = (IndexExpression *)expression;

      printf_expression(index_expression->left_expression, alignment);
      printf("[");
      printf_expression(index_expression->right_expression, alignment);
      printf("] ");

      break;
    }

    case ExpressionTypeCallExpression: {
      CallExpression *call_expression = (CallExpression *) expression;

      printf(" `");
      printf_expression(call_expression->identifier_expression, alignment);
      printf("->");
      printf_expression(call_expression->tuple_expression, alignment);
      printf("` ");

      break;
    }

    case ExpressionTypeArrayExpression: {
      ArrayExpression *array_expression = (ArrayExpression *) expression;

      if (array_expression->array_expression_type == ArrayExpressionTypeList) {
        printf(" [");
      } else {
        printf(" T(");
      }

      for (size_t i = 0; i < array_expression->expression_count; i++) {
        printf_expression(array_expression->expressions[i], alignment);

        if (i < array_expression->expression_count - 1) {
          printf(",");
        }
      }

      if (array_expression->array_expression_type == ArrayExpressionTypeList) {
        printf("] ");
      } else {
        printf(") ");
      }

      break;
    }

    case ExpressionTypeFunctionExpression: {
      FunctionExpression *function_expression = (FunctionExpression *)expression;

      printf("FUNCTION");

      if (function_expression->identifier) printf_expression(function_expression->identifier, alignment);
      printf("(");
      for (size_t i = 0; i < function_expression->argument_count; i++) {
        printf("%s", function_expression->arguments[i]);

        if (i != function_expression->argument_count - 1) {
          printf(", ");
        }
      }
      printf(")");
      printf("\n");
      printf_block(function_expression->block, alignment);

      break;
    }

    case ExpressionTypeBoolExpression: {
      printf(" %s ", ((BoolLiteral *)expression->literal)->bool_literal ? "true" : "false");

      break;
    }

    case ExpressionTypeIntegerExpression: {
      printf(" %lld ", ((IntegerLiteral *)expression->literal)->integer_literal);

      break;
    }

    case ExpressionTypeFloatExpression: {
      printf(" %Lf ", ((FloatLiteral *)expression->literal)->float_literal);

      break;
    }

    case ExpressionTypeStringExpression: {
      printf(" \"%s\" ", ((StringLiteral *)expression->literal)->string_literal);

      break;
    }

    case ExpressionTypeIdentifierExpression: {
      printf(" %s ", ((StringLiteral *)expression->literal)->string_literal);

      break;
    }

    case ExpressionTypeNullExpression: {
      printf(" %s ", "null");

      break;
    }


    default : {

    }
  }

}


void printf_block(Block *block, unsigned int alignment) {
  printf("{\n");

  for (size_t i = 0; i < block->statement_count; i++) {
    printf_statement(block->statements[i], alignment + 1u);
  }

  printf("}\n");
}


void printf_block_definition(BlockDefinition *block_definition, unsigned int alignment) {
  switch (block_definition->block_definition_type) {
    case BlockDefinitionTypeIfElseGroupBlock: {
      IfElseGroupBlockDefinition *if_else_group_block_definition = (IfElseGroupBlockDefinition *)block_definition;

      for (size_t i = 0; i < if_else_group_block_definition->if_block_definitions_length; i++) {
        printf_block_definition((BlockDefinition *) if_else_group_block_definition->if_block_definitions[i], alignment);
      }

      if (if_else_group_block_definition->else_block_definition) {
        printf_block_definition((BlockDefinition *) if_else_group_block_definition->else_block_definition, alignment);
      }

      break;
    }

    case BlockDefinitionTypeIfBlock: {
      IfBlockDefinition *if_block_definition = (IfBlockDefinition *)block_definition;

      printf_alignment(alignment);
      printf("IF");

      if (if_block_definition->pre_expression) {
        printf_expression(if_block_definition->pre_expression, alignment);
        printf(";");
      }

      printf_expression(if_block_definition->condition, alignment);
      printf("\n");
      printf_block(if_block_definition->block, alignment);

      break;
    }

    case BlockDefinitionTypeElseBlock: {
      ElseBlockDefinition *else_block_definition = (ElseBlockDefinition *) block_definition;
      printf_block(else_block_definition->block, alignment);

      break;
    }

    case BlockDefinitionTypeForBlock: {
      ForBlockDefinition *for_block_definition = (ForBlockDefinition *)block_definition;

      printf_alignment(alignment);
      printf("FOR");

      if (for_block_definition->pre_expression) {
        printf_expression(for_block_definition->pre_expression, alignment);
        printf(";");
      }

      printf_expression(for_block_definition->condition, alignment);

      if (for_block_definition->post_expression) {
        printf(";");
        printf_expression(for_block_definition->post_expression, alignment);
      }
      printf("\n");
      printf_block(for_block_definition->block, alignment);

      break;
    }
  }
}


void printf_statement(Statement *statement, unsigned int alignment) {
  switch (statement->statement_type) {
    case StatementTypeReturnStatement: {
      ReturnStatement *return_statement = (ReturnStatement *)statement;

      printf_alignment(alignment);
      printf("return ");
      printf_expression(return_statement->right_expression, alignment);
      printf("\n");

      break;
    }

    case StatementTypeImportStatement: {
      ImportStatement *import_statement = (ImportStatement *)statement;

      printf_alignment(alignment);
      printf("import ");
      printf_expression(import_statement->right_expression, alignment);
      printf("\n");

      break;
    }

    case StatementTypeBlockDefinitionStatement: {
      BlockDefinitionStatement *block_definition_statement = (BlockDefinitionStatement *)statement;

      printf_block_definition(block_definition_statement->block_definition, alignment);

      break;
    }

    case StatementTypeExpressionStatement: {
      ExpressionStatement *expression_Statement = (ExpressionStatement *)statement;

      printf_alignment(alignment);
      printf_expression(expression_Statement->expression, alignment);
      printf("\n");

      break;
    }

    default : {

    }
  }
}


void printf_ast(AST *ast) {
  for (size_t i = 0; i < ast->block->statement_count; i++) {
    printf_statement(ast->block->statements[i], 0);
  }
  printf("\n");
}


void free_expression(Expression *expression) {
  switch (expression->expression_type) {
    case ExpressionTypeNullExpression: {
      break;
    }

    case ExpressionTypeIndexExpression: {
      IndexExpression *index_expression = (IndexExpression *) expression;

      free_expression(index_expression->left_expression);
      free_expression(index_expression->right_expression);
      free(index_expression);

      break;
    }

    case ExpressionTypeInfixExpression: {
      InfixExpression *infix_expression = (InfixExpression *)expression;

      free_expression(infix_expression->left_expression);
      free_expression(infix_expression->right_expression);
      free(infix_expression);

      break;
    }

    case ExpressionTypePrefixExpression: {
      PrefixExpression *prefix_expression = (PrefixExpression *)expression;

      free_expression(prefix_expression->right_expression);
      free(prefix_expression);

      break;
    }

    case ExpressionTypeCallExpression: {
      CallExpression *call_expression = (CallExpression *)expression;

      free_expression(call_expression->identifier_expression);
      free_expression(call_expression->tuple_expression);
      free(call_expression);

      break;
    }

    case ExpressionTypeArrayExpression: {
      ArrayExpression *array_expression = (ArrayExpression *)expression;

      for (size_t i = 0; i < array_expression->expression_count; i++) {
        free_expression(array_expression->expressions[i]);
      }

      free(array_expression->expressions);
      free(array_expression);

      break;
    }

    case ExpressionTypeFunctionExpression: {
      FunctionExpression *function_expression = (FunctionExpression *)expression;

      for (size_t i = 0; i < function_expression->argument_count; i++) {
        free(function_expression->arguments[i]);
      }
      free(function_expression->arguments);

      if (function_expression->identifier) free_expression(function_expression->identifier);
      free_block(function_expression->block);
      free(function_expression);

      break;
    }

    case ExpressionTypeBoolExpression:
    case ExpressionTypeIntegerExpression:
    case ExpressionTypeFloatExpression: {
      free(expression->literal);
      free(expression);
      break;
    }

    case ExpressionTypeStringExpression:
    case ExpressionTypeIdentifierExpression: {
      free(((StringLiteral *) expression->literal)->string_literal);
      free(expression->literal);
      free(expression);
      break;
    }
  }
}


void free_block(Block *block) {
  for (size_t i = 0; i < block->statement_count; i++) {
    free_statement(block->statements[i]);
  }
  free(block->statements);
  free(block);
}


void free_block_definition(BlockDefinition *block_definition) {
  switch (block_definition->block_definition_type) {
    case BlockDefinitionTypeIfElseGroupBlock: {
      IfElseGroupBlockDefinition *if_else_group_block_definition = (IfElseGroupBlockDefinition *)block_definition;

      for (size_t i = 0; i < if_else_group_block_definition->if_block_definitions_length; i++) {
        free_block_definition((BlockDefinition *) if_else_group_block_definition->if_block_definitions[i]);
      }
      free(if_else_group_block_definition->if_block_definitions);

      if (if_else_group_block_definition->else_block_definition) free_block_definition((BlockDefinition *) if_else_group_block_definition->else_block_definition);
      free(if_else_group_block_definition);

      break;
    }

    case BlockDefinitionTypeIfBlock: {
      IfBlockDefinition *if_block_definition = (IfBlockDefinition *)block_definition;

      free_expression(if_block_definition->condition);
      if (if_block_definition->pre_expression != NULL) free_expression(if_block_definition->pre_expression);
      free_block(if_block_definition->block);
      free(if_block_definition);

      break;
    }

    case BlockDefinitionTypeElseBlock: {
      break;
    }

    case BlockDefinitionTypeForBlock: {
      ForBlockDefinition *for_block_definition = (ForBlockDefinition *)block_definition;

      free_expression(for_block_definition->condition);
      if (for_block_definition->pre_expression != NULL) free_expression(for_block_definition->pre_expression);
      if (for_block_definition->post_expression != NULL) free_expression(for_block_definition->post_expression);
      free_block(for_block_definition->block);
      free(for_block_definition);

      break;
    }

    default : {

    }
  }
}


void free_statement(Statement *statement) {
  switch (statement->statement_type) {
    case StatementTypeExpressionStatement: {
      ExpressionStatement *expression_statement = (ExpressionStatement *)statement;

      free_expression(expression_statement->expression);
      free(expression_statement);

      break;
    }

    case StatementTypeImportStatement: {
      ImportStatement *import_statement = (ImportStatement *)statement;

      free_expression(import_statement->right_expression);
      free(import_statement);
      break;
    }

    case StatementTypeReturnStatement: {
      ReturnStatement *return_statement = (ReturnStatement *)statement;

      free_expression(return_statement->right_expression);
      free(return_statement);
      break;
    }

    case StatementTypeBlockDefinitionStatement: {
      BlockDefinitionStatement *block_definition_statement = (BlockDefinitionStatement *)statement;

      free_block_definition(block_definition_statement->block_definition);
      free(block_definition_statement);
      break;
    }
  }
}


void free_ast(AST *ast) {
  // no need to free block, because scope automatically frees it, TODO move maybe again here
  free_block(ast->block);
  free(ast);
}


Operator token_to_operator(Token token) {
  switch (token.token_type) {
    case EQUAL_TOKEN:
      return ASSIGN_OP;

    case PLUS_TOKEN:
      return ADDITION_OP;

    case PLUS_EQUAL_TOKEN:
      return ASSIGN_ADDITION_OP;

    case MINUS_TOKEN:
      return SUBTRACTION_OP;

    case MINUS_EQUAL_TOKEN:
      return ASSIGN_SUBTRACTION_OP;

    case STAR_TOKEN:
      return MULTIPLICATION_OP;

    case STAR_EQUAL_TOKEN:
      return ASSIGN_MULTIPLICATION_OP;

    case DOUBLE_STAR_TOKEN:
      return EXPONENT_OP;

    case DOUBLE_STAR_EQUAL_TOKEN:
      return ASSIGN_EXPONENT_OP;

    case SLASH_TOKEN:
      return DIVISION_OP;

    case SLASH_EQUAL_TOKEN:
      return ASSIGN_DIVISION_OP;

    case DOUBLE_SLASH_TOKEN:
      return INTEGER_DIVISION_OP;

    case DOUBLE_SLASH_EQUAL_TOKEN:
      return ASSIGN_INTEGER_DIVISION_OP;

    case PERCENTAGE_TOKEN:
      return MOD_OP;

    case PERCENTAGE_EQUAL_TOKEN:
      return ASSIGN_MOD_OP;

    case CARROT_TOKEN:
      return EXPONENT_OP;

    case CARROT_EQUAL_TOKEN:
      return ASSIGN_EXPONENT_OP;

    case DOUBLE_EQUAL_TOKEN:
      return CHECK_EQUALITY_OP;

    case EXCLAMATION_EQUAL_TOKEN:
      return CHECK_NOT_EQUALITY_OP;

    case SMALLER_TOKEN:
      return CHECK_SMALLER_OP;

    case SMALLER_EQUAL_TOKEN:
      return CHECK_SMALLER_EQUAL_OP;

    case BIGGER_TOKEN:
      return CHECK_BIGGER_OP;

    case BIGGER_EQUAL_TOKEN:
      return CHECK_BIGGER_EQUAL_OP;

    case L_PARENTHESIS_TOKEN:
      return L_PARENTHESIS_OP;

    case L_BRACKET_TOKEN:
      return L_BRACKET_OP;

    case EXCLAMATION_TOKEN:
      return NOT_OP;

    case IN_TOKEN:
      return IN_OP;

    case COMMA_TOKEN:
      return COMMA_OP;

    case RANGE_TOKEN:
      return RANGE_OP;

    case ASYNC_TOKEN:
      return ASYNC_OP;

    case AWAIT_TOKEN:
      return AWAIT_OP;

    default:
      return -1;
  }
}


bool check_if_token_is_operator(Token token) {
  return token_to_operator(token) != -1;
}


bool is_operator_right_associative(Operator operator) {
  return operator == EXPONENT_OP || operator == ASSIGN_OP;
}


unsigned short get_operator_precedence(Operator operator, bool next) {
  unsigned short result;

  switch (operator) {
    case ASSIGN_OP:
    case ASSIGN_ADDITION_OP:
    case ASSIGN_SUBTRACTION_OP:
    case ASSIGN_MULTIPLICATION_OP:
    case ASSIGN_DIVISION_OP:
    case ASSIGN_INTEGER_DIVISION_OP:
    case ASSIGN_MOD_OP:
    case ASSIGN_EXPONENT_OP: {
      result = ASSIGN_PRECEDENCE;
      break;
    }

    case CHECK_EQUALITY_OP:
    case CHECK_NOT_EQUALITY_OP:
    case CHECK_BIGGER_OP:
    case CHECK_BIGGER_EQUAL_OP:
    case CHECK_SMALLER_OP:
    case CHECK_SMALLER_EQUAL_OP: {
      result = CONDITIONAL_PRECEDENCE;
      break;
    }

    case ADDITION_OP:
    case SUBTRACTION_OP: {
      result = ADDITION_SUBTRACTION_PRECEDENCE;
      break;
    }

    case MULTIPLICATION_OP:
    case DIVISION_OP:
    case INTEGER_DIVISION_OP:
    case MOD_OP: {
      result = MULTIPLICATION_DIVISION_MOD_PRECEDENCE;
      break;
    }

    case EXPONENT_OP: {
      result = EXPONENT_PRECEDENCE;
      break;
    }

    case L_PARENTHESIS_OP: {
      result = CALL_PRECEDENCE;
      break;
    }

    case L_BRACKET_OP: {
      result = LIST_PRECEDENCE;
      break;
    }


    case IN_OP: {
      result = IN_OP_PRECEDENCE;
      break;
    }

    case RANGE_OP: {
      result = RANGE_PRECEDENCE;
      break;
    }

    case COMMA_OP: {
      result = COMMA_PRECEDENCE;
      break;
    }

    case ASYNC_OP:
    case AWAIT_OP: {
      result = ASYNC_AWAIT_PRECEDENCE;
      break;
    }

    default: {
      result = 0;
    }
  }

  if (next && is_operator_right_associative(operator)) result -= 1u;

  return result;
}


// parser starts here


bool has_finished(Token token, ParserLimiter limiter) {
  if (token.token_type == EOF_TOKEN) {
    return true;
  }

  switch (limiter) {
    case DEFAULT_BLOCK_PARSER_LIMITER:
      return token.token_type == R_BRACE_TOKEN;

    case DEFAULT_EXPRESSION_PARSER_LIMITER:
      return token.token_type == EOL_TOKEN;

    case GROUPED_EXPRESSION_PARSER_LIMITER:
      return token.token_type == R_PARENTHESIS_TOKEN;

    case ARRAY_EXPRESSION_PARSER_LIMITER:
      return token.token_type == R_BRACKET_TOKEN || token.token_type == R_PARENTHESIS_TOKEN;

    case INDEX_EXPRESSION_PARSER_LIMITER:
      return token.token_type == R_BRACKET_TOKEN;

    case IF_BLOCK_EXPRESSION_PARSER_LIMITER:
    case FOR_BLOCK_EXPRESSION_PARSER_LIMITER:
      return token.token_type == L_BRACE_TOKEN || token.token_type == SEMICOLON_TOKEN;

    default:
      return false;
  }
}


void *parser_error() {
  raise(SIGUSR1);

  return NULL;
}


void *parser_error_undefined_position() { //Lexer *lexer
  printf("Undefined position of token ");

  //printf_token(peek_token(lexer));

  // at line

  parser_error();
}


Expression *eval_token(Token token) {
  static Expression *null_expression = NULL;

  if (token.token_type == NULL_TOKEN) {
    if (null_expression == NULL) {
      null_expression = malloc(sizeof(Expression));

      null_expression->literal = NULL;
      null_expression->expression_type = ExpressionTypeNullExpression;
    }

    return null_expression;
  }

  Expression *expression = malloc(sizeof(Expression));
  expression->literal = token.literal;

  switch (token.token_type) {
    case IDENTIFIER_TOKEN: {
      expression->expression_type = ExpressionTypeIdentifierExpression;
      break;
    }

    case INTEGER_TOKEN: {
      expression->expression_type = ExpressionTypeIntegerExpression;
      break;
    }

    case FLOAT_TOKEN: {
      expression->expression_type = ExpressionTypeFloatExpression;
      break;
    }

    case BOOL_TOKEN: {
      expression->expression_type = ExpressionTypeBoolExpression;
      break;
    }

    case STRING_LITERAL_TOKEN: {
      expression->expression_type = ExpressionTypeStringExpression;
      break;
    }

    default: {
      return parser_error_undefined_position();
    }
  }

  return expression;
}


Expression *force_array_expression(Expression *expression, ArrayExpressionType array_expression_type) {
  if (expression == NULL) {
    ArrayExpression *array_expression = malloc(sizeof(ArrayExpression));

    array_expression->expression = (Expression){.expression_type = ExpressionTypeArrayExpression};
    array_expression->expression_count = 0;
    array_expression->expressions = malloc(0);
    array_expression->array_expression_type = array_expression_type;
    array_expression->has_finished = false;

    return (Expression *) array_expression;
  }
  else if (expression->expression_type == ExpressionTypeArrayExpression) {
    if (!((ArrayExpression *) expression)->has_finished) {
      ((ArrayExpression *) expression)->array_expression_type = array_expression_type;

      return expression;
    }

    ArrayExpression *array_expression = malloc(sizeof(ArrayExpression));

    array_expression->expression = (Expression){.expression_type = ExpressionTypeArrayExpression};
    array_expression->expression_count = 1;
    array_expression->expressions = malloc(sizeof(Expression *));
    array_expression->array_expression_type = array_expression_type;
    array_expression->has_finished = false;

    array_expression->expressions[0] = expression;

    return (Expression *) array_expression;
  }
  else  {
    ArrayExpression *array_expression = malloc(sizeof(ArrayExpression));

    array_expression->expression = (Expression){.expression_type = ExpressionTypeArrayExpression};
    array_expression->expression_count = 1;
    array_expression->expressions = malloc(sizeof(Expression *));
    array_expression->array_expression_type = array_expression_type;
    array_expression->has_finished = false;

    array_expression->expressions[0] = expression;

    return (Expression *) array_expression;
  }
}


Expression *parse_grouped_expression(Lexer *lexer) {
  if (peek_token(lexer).token_type != L_PARENTHESIS_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  Expression *result = parse_expression(lexer, 0, GROUPED_EXPRESSION_PARSER_LIMITER);

  if (result->expression_type == ExpressionTypeArrayExpression) {
    ArrayExpression *array_expression = (ArrayExpression *) result;

    array_expression->has_finished = true;
  }

  if (peek_token(lexer).token_type != R_PARENTHESIS_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  return result;
}


Expression *parse_list_expression(Lexer *lexer) {
  if (peek_token(lexer).token_type != L_BRACKET_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  Expression *result = force_array_expression(parse_expression(lexer, 0, ARRAY_EXPRESSION_PARSER_LIMITER), ArrayExpressionTypeList);

  ArrayExpression *array_expression = (ArrayExpression *)result;
  array_expression->array_expression_type = ArrayExpressionTypeList;
  array_expression->has_finished = true;

  if (peek_token(lexer).token_type != R_BRACKET_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  return result;
}


Expression *parse_prefix_expression(Lexer *lexer, ParserLimiter limiter) {
  Token curr_token = peek_token(lexer);
  if (has_finished(curr_token, limiter)) return NULL; // TODO

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_grouped_expression(lexer);
  } else if (curr_token.token_type == L_BRACKET_TOKEN) {
    return parse_list_expression(lexer);
  }

  next_token(lexer);

  PrefixExpression *prefix_expression = malloc(sizeof(PrefixExpression));

  prefix_expression->expression = (Expression){.expression_type = ExpressionTypePrefixExpression};
  prefix_expression->operator = token_to_operator(curr_token);
  prefix_expression->right_expression = parse_expression(lexer, get_operator_precedence(prefix_expression->operator, true), limiter);

  return (Expression *)prefix_expression;
}


Expression *parse_array_expression(Lexer *lexer, Expression *left) {
  ArrayExpression *array_expression = NULL;

  if (left->expression_type == ExpressionTypeArrayExpression && !(((ArrayExpression *) left)->has_finished)) {
    array_expression = (ArrayExpression *)left;
  }
  else {
    array_expression = malloc(sizeof(ArrayExpression));

    array_expression->expression = (Expression){.expression_type = ExpressionTypeArrayExpression};
    array_expression->expression_count = 1;
    array_expression->expressions = malloc(array_expression->expression_count * sizeof(Expression *));
    array_expression->array_expression_type = ArrayExpressionTypeTuple;
    array_expression->has_finished = false;

    array_expression->expressions[0] = left;

  }

  if (has_finished(peek_token(lexer), ARRAY_EXPRESSION_PARSER_LIMITER)) return (Expression *) array_expression;

  Expression *right = parse_expression(lexer, COMMA_PRECEDENCE, ARRAY_EXPRESSION_PARSER_LIMITER);

  if (right != NULL) {
    array_expression->expressions = realloc(array_expression->expressions, sizeof(Expression *) * (++array_expression->expression_count));

    array_expression->expressions[array_expression->expression_count - 1] = right;
  }

  return (Expression *)array_expression;
}


Expression *parse_index_expression(Lexer *lexer, Expression *identifier) {
  if (peek_token(lexer).token_type != L_BRACKET_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  Expression *right_expression = parse_expression(lexer, 0, INDEX_EXPRESSION_PARSER_LIMITER);

  if (peek_token(lexer).token_type != R_BRACKET_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  IndexExpression *index_expression = malloc(sizeof(IndexExpression));

  index_expression->expression = (Expression) {.expression_type = ExpressionTypeIndexExpression};
  index_expression->left_expression = identifier;
  index_expression->right_expression = right_expression;

  return (Expression *) index_expression;
}


Expression *parse_call_expression(Lexer *lexer, Expression *identifier) {
  if (peek_token(lexer).token_type != L_PARENTHESIS_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  CallExpression *call_expression = malloc(sizeof(CallExpression));
  call_expression->expression = (Expression){.expression_type = ExpressionTypeCallExpression};
  call_expression->identifier_expression = identifier;
  call_expression->tuple_expression = force_array_expression(
    parse_expression(lexer, 0, GROUPED_EXPRESSION_PARSER_LIMITER), ArrayExpressionTypeTuple);

  if (peek_token(lexer).token_type != R_PARENTHESIS_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  return (Expression *)call_expression;
}


Expression *parse_infix_expression(Lexer *lexer, ParserLimiter limiter, Expression *left) {
  Token curr_token = peek_token(lexer);
  if (has_finished(curr_token, limiter)) return left;

  if (curr_token.token_type == L_PARENTHESIS_TOKEN) {
    return parse_call_expression(lexer, left);
  }
  else if (curr_token.token_type == L_BRACKET_TOKEN) {
    return parse_index_expression(lexer, left);
  }
  else if (curr_token.token_type == COMMA_TOKEN) {
    next_token(lexer);
    return parse_array_expression(lexer, left);
  }

  next_token(lexer);

  InfixExpression *infix_expression = malloc(sizeof(InfixExpression));
  infix_expression->expression = (Expression){.expression_type = ExpressionTypeInfixExpression};
  infix_expression->left_expression = left;
  infix_expression->operator = token_to_operator(curr_token);
  infix_expression->right_expression = parse_expression(lexer, get_operator_precedence(infix_expression->operator, true), limiter);

  return (Expression *)infix_expression;
}


Expression *parse_function_expression(Lexer *lexer) {
  next_token(lexer);

  Expression *identifier = NULL;

  if (peek_token(lexer).token_type != L_PARENTHESIS_TOKEN) identifier = eval_token(next_token(lexer));

  if (peek_token(lexer).token_type != L_PARENTHESIS_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  ArrayExpression *arguments_expression = (ArrayExpression *) force_array_expression(parse_expression(lexer, 0, GROUPED_EXPRESSION_PARSER_LIMITER), ArrayExpressionTypeTuple);

  if (peek_token(lexer).token_type != R_PARENTHESIS_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  if (peek_token(lexer).token_type != L_BRACE_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  Block *block = parse_block(lexer, DEFAULT_BLOCK_PARSER_LIMITER);

  if (peek_token(lexer).token_type != R_BRACE_TOKEN) return parser_error_undefined_position();
  next_token(lexer);

  char **arguments = malloc(arguments_expression->expression_count * sizeof(char *));

  for (size_t i = 0; i < arguments_expression->expression_count; i++) {
    arguments[i] = copy_string(((StringLiteral *) arguments_expression->expressions[i]->literal)->string_literal);
  }

  FunctionExpression *function_expression = (FunctionExpression *)malloc(sizeof(FunctionExpression));
  function_expression->expression = (Expression){.expression_type = ExpressionTypeFunctionExpression};
  function_expression->block = block;
  function_expression->identifier = identifier;
  function_expression->arguments = arguments;
  function_expression->argument_count = arguments_expression->expression_count;

  free_expression((Expression *) arguments_expression);

  return (Expression *)function_expression;
}


Expression *parse_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter) {
  Token curr_token = peek_token(lexer);

  if (has_finished(curr_token, limiter)) return NULL;

  if (curr_token.token_type == FUNCTION_TOKEN) return parse_function_expression(lexer);

  Expression *left;

  if (check_if_token_is_operator(curr_token)) left = parse_prefix_expression(lexer, limiter);
  else left = eval_token(next_token(lexer));

  curr_token = peek_token(lexer);
  if (has_finished(curr_token, limiter)) return left;

  while (precedence < get_operator_precedence(token_to_operator(curr_token), false)) {
    if (has_finished(curr_token, limiter)) return left;

    curr_token = peek_token(lexer);

    if (has_finished(curr_token, limiter)) return left;
    else if (check_if_token_is_operator(curr_token)) {
      left = parse_infix_expression(lexer, limiter, left);
      curr_token = peek_token(lexer);

    }
    else return left;
  }

  return left;
}


Block *parse_block(Lexer *lexer, ParserLimiter limiter) {
  Statement *items_buffer[MAX_BUFFER_SIZE];
  size_t items_buffer_length = 0;

  Block *block = malloc(sizeof(Block));

  block->statement_count = 0;
  block->statements = malloc(block->statement_count * sizeof(Statement *));

  while (true) {
    while (peek_token(lexer).token_type == EOL_TOKEN) { next_token(lexer); }

    if (has_finished(peek_token(lexer), limiter)) { break; }

    if (items_buffer_length == MAX_BUFFER_SIZE) {
      block->statements = realloc(block->statements, block->statement_count * sizeof(Statement *));

      memcpy(&block->statements[block->statement_count - items_buffer_length], items_buffer, items_buffer_length * sizeof(Statement *));

      items_buffer_length = 0;
    }

    Statement *statement = parse_statement(lexer);

    if (statement == NULL) continue;

    items_buffer[items_buffer_length++] = statement;
    block->statement_count++;
  }

  block->statements = realloc(block->statements, block->statement_count * sizeof(Statement *));

  if (block->statement_count > MAX_BUFFER_SIZE) {
    memcpy(&block->statements[block->statement_count - items_buffer_length], items_buffer, items_buffer_length * sizeof(Statement *));
  }
  else if (block->statement_count < MAX_BUFFER_SIZE) {
    memcpy(block->statements, items_buffer, items_buffer_length * sizeof(Statement *));
  }

  return block;
}


BlockDefinition *parse_if_block_definition(Lexer *lexer, ParserLimiter limiter) {
  IfElseGroupBlockDefinition *if_else_group_block_definition = malloc(sizeof(IfElseGroupBlockDefinition));

  if_else_group_block_definition->block_definition = (BlockDefinition) {.block_definition_type = BlockDefinitionTypeIfElseGroupBlock};
  if_else_group_block_definition->else_block_definition = NULL;
  if_else_group_block_definition->if_block_definitions_length = 0;
  if_else_group_block_definition->if_block_definitions = malloc(0);

  while (peek_token(lexer).token_type == IF_TOKEN) {
    next_token(lexer);

    Expression *expression1 = NULL;
    Expression *expression2 = NULL;

    expression1 = parse_expression(lexer, 0, IF_BLOCK_EXPRESSION_PARSER_LIMITER);

    if (peek_token(lexer).token_type == SEMICOLON_TOKEN) {
      next_token(lexer);

      expression2 = parse_expression(lexer, 0, IF_BLOCK_EXPRESSION_PARSER_LIMITER);
    }

    if (peek_token(lexer).token_type != L_BRACE_TOKEN) return parser_error_undefined_position();
    next_token(lexer);

    Block *block = parse_block(lexer, limiter);

    if (peek_token(lexer).token_type != R_BRACE_TOKEN) return parser_error_undefined_position();
    next_token(lexer);

    IfBlockDefinition *if_block_definition = malloc(sizeof(IfBlockDefinition));
    if_block_definition->block_definition = (BlockDefinition){.block_definition_type = BlockDefinitionTypeIfBlock};
    if_block_definition->block = block;

    if (expression2 == NULL) {
      if_block_definition->pre_expression = NULL;
      if_block_definition->condition = expression1;
    }
    else {
      if_block_definition->pre_expression = expression1;
      if_block_definition->condition = expression2;
    }

    if_else_group_block_definition->if_block_definitions = realloc(if_else_group_block_definition->if_block_definitions, (if_else_group_block_definition->if_block_definitions_length + 1) * sizeof(IfBlockDefinition *));
    if_else_group_block_definition->if_block_definitions[if_else_group_block_definition->if_block_definitions_length++] = if_block_definition;
  }

  if (peek_token(lexer).token_type == ELSE_TOKEN) {
    next_token(lexer);

    if (peek_token(lexer).token_type != L_BRACE_TOKEN) return parser_error_undefined_position();
    next_token(lexer);

    Block *block = parse_block(lexer, limiter);

    if (peek_token(lexer).token_type != R_BRACE_TOKEN) return parser_error_undefined_position();
    next_token(lexer);

    ElseBlockDefinition *else_block_definition = malloc(sizeof(ElseBlockDefinition));

    else_block_definition->block_definition = (BlockDefinition) {.block_definition_type = BlockDefinitionTypeElseBlock};
    else_block_definition->block = block;

    if_else_group_block_definition->else_block_definition = else_block_definition;
  }

  return (BlockDefinition *) if_else_group_block_definition;
}


BlockDefinition *parse_for_block_definition(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  Expression *expression1 = NULL;
  Expression *expression2 = NULL;
  Expression *expression3 = NULL;

  expression1 = parse_expression(lexer, 0, FOR_BLOCK_EXPRESSION_PARSER_LIMITER);

  if (peek_token(lexer).token_type == SEMICOLON_TOKEN) {
    next_token(lexer);

    expression2 = parse_expression(lexer, 0, FOR_BLOCK_EXPRESSION_PARSER_LIMITER);

    if (peek_token(lexer).token_type == SEMICOLON_TOKEN) {
      next_token(lexer);

      expression3 = parse_expression(lexer, 0, FOR_BLOCK_EXPRESSION_PARSER_LIMITER);
    }
  }

  if (peek_token(lexer).token_type != L_BRACE_TOKEN) return parser_error_undefined_position();

  next_token(lexer);

  Block *block = parse_block(lexer, limiter);

  if (peek_token(lexer).token_type != R_BRACE_TOKEN) return parser_error_undefined_position();

  next_token(lexer);

  ForBlockDefinition *for_block_definition = malloc(sizeof(ForBlockDefinition));
  for_block_definition->block_definition = (BlockDefinition){.block_definition_type = BlockDefinitionTypeForBlock};
  for_block_definition->block = block;

  if (expression2 == NULL) {
    for_block_definition->pre_expression = NULL;
    for_block_definition->condition = expression1;
    for_block_definition->post_expression = NULL;
  }
  else if (expression3 == NULL) {
    for_block_definition->pre_expression = expression1;
    for_block_definition->condition = expression2;
    for_block_definition->post_expression = NULL;
  }
  else {
    for_block_definition->pre_expression = expression1;
    for_block_definition->condition = expression2;
    for_block_definition->post_expression = expression3;
  }

  return (BlockDefinition *)for_block_definition;
}


BlockDefinition *parse_block_definition(Lexer *lexer, ParserLimiter limiter) {
  Token token = peek_token(lexer);

  if (token.token_type == IF_TOKEN) {
    return parse_if_block_definition(lexer, limiter);
  }
  else if (token.token_type == FOR_TOKEN) {
    return parse_for_block_definition(lexer, limiter);
  }

  return NULL;
}


Statement *parse_block_definition_statement(Lexer *lexer, ParserLimiter limiter) {
  BlockDefinitionStatement *block_definition_statement = malloc(sizeof(BlockDefinitionStatement));

  block_definition_statement->statement = (Statement){.statement_type = StatementTypeBlockDefinitionStatement};
  block_definition_statement->block_definition = parse_block_definition(lexer, limiter);

  return (Statement *)block_definition_statement;
}


Statement *parse_return_statement(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  ReturnStatement *return_statement = malloc(sizeof(ReturnStatement));
  return_statement->statement = (Statement){.statement_type = StatementTypeReturnStatement};
  return_statement->right_expression = parse_expression(lexer, 0, limiter);

  return (Statement *)return_statement;
}


Statement *parse_import_statement(Lexer *lexer, ParserLimiter limiter) {
  next_token(lexer);

  ImportStatement *import_statement = malloc(sizeof(ImportStatement));
  import_statement->statement = (Statement){.statement_type = StatementTypeImportStatement};
  import_statement->right_expression = parse_expression(lexer, 0, limiter);

  return (Statement *)import_statement;
}


Statement *parse_expression_statement(Lexer *lexer, ParserLimiter limiter) {
  ExpressionStatement *expression_statement = malloc(sizeof(ExpressionStatement));
  expression_statement->statement = (Statement){.statement_type = StatementTypeExpressionStatement};
  expression_statement->expression = parse_expression(lexer, 0, limiter);

  // next_token(lexer); // todo i removed but idk that what it does

  return (Statement *)expression_statement;
}


Statement *parse_statement(Lexer *lexer) {
  Token token = peek_token(lexer);

  Statement *statement;

  if (token.token_type == EOF_TOKEN) {
    return NULL;
  }
  else if (token.token_type == RETURN_TOKEN) {
    statement = parse_return_statement(lexer, DEFAULT_EXPRESSION_PARSER_LIMITER);
  }
  else if (token.token_type == IMPORT_TOKEN) {
    statement = parse_import_statement(lexer, DEFAULT_EXPRESSION_PARSER_LIMITER);
  }
  else if (token.token_type == IF_TOKEN || token.token_type == FOR_TOKEN) {
    statement = parse_block_definition_statement(lexer, DEFAULT_BLOCK_PARSER_LIMITER);
  }
  else {
    statement = parse_expression_statement(lexer, DEFAULT_EXPRESSION_PARSER_LIMITER);
  }

  return statement;
}


AST *parse_ast(Lexer *lexer) {
  AST *ast = malloc(sizeof(AST));

  ast->block = parse_block(lexer, DEFAULT_BLOCK_PARSER_LIMITER);

  return ast;
}