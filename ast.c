#include "ast.h"

#include "bool.h"
#include "lexer.h"

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
      InfixExpression *infix_expression = (InfixExpression *)expression;

      printf(" (");
      printf_expression(infix_expression->left_expression, alignment);

      if (infix_expression->operator == ADDITION_OP) { printf(" + "); }
      else if (infix_expression->operator == SUBTRACTION_OP) { printf(" - "); }
      else if (infix_expression->operator == MULTIPLICATION_OP) { printf(" * "); }
      else if (infix_expression->operator == DIVISION_OP) { printf(" / "); }
      else if (infix_expression->operator == MOD_OP) { printf(" %% "); }
      else if (infix_expression->operator == EXPONENT_OP) { printf(" ^ "); }
      else if (infix_expression->operator == ASSIGN_OP) { printf(" = "); }
      else if (infix_expression->operator == MEMBER_OP) { printf(" . "); }
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
      PrefixExpression *prefix_expression = (PrefixExpression *)expression;

      printf(" (");

      if (prefix_expression->operator == NOT_OP) { printf("!"); }
      else if (prefix_expression->operator == ADDITION_OP) { printf("+"); }
      else if (prefix_expression->operator == SUBTRACTION_OP) { printf("-"); }
      else if (prefix_expression->operator == PLUS_PLUS_OP) { printf("++"); }
      else if (prefix_expression->operator == MINUS_MINUS_OP) { printf("--"); }
      else if (prefix_expression->operator == ASYNC_OP) { printf(" async "); }
      else if (prefix_expression->operator == AWAIT_OP) { printf(" await "); }

      printf_expression(prefix_expression->right_expression, alignment);
      printf(") ");

      break;
    }

    case ExpressionTypePostfixExpression: {
      PostfixExpression *postfix_expression = (PostfixExpression *)expression;

      printf(" (");
      printf_expression(postfix_expression->left_expression, alignment);

      if (postfix_expression->operator == PLUS_PLUS_OP) { printf("++"); }
      else if (postfix_expression->operator == MINUS_MINUS_OP) { printf("--"); }

      printf(") ");

      break;
    }

    case ExpressionTypeCallExpression: {
      CallExpression *call_expression = (CallExpression *)expression;

      printf(" `");
      printf_expression(call_expression->identifier_expression, alignment);
      printf("->");
      printf_expression(call_expression->tuple_expression, alignment);
      printf("` ");

      break;
    }

    case ExpressionTypeArrayExpression: {
      ArrayExpression *array_expression = (ArrayExpression *)expression;

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

    case ExpressionTypeMemberExpression: {
      MemberExpression *member_expression = (MemberExpression *)expression;

      printf(" (");
      for (size_t i = 0; i < member_expression->expression_count; i++) {
        printf_expression(member_expression->expressions[i], alignment);

        if (i < member_expression->expression_count - 1) {
          printf(".");
        }
      }
      printf(") ");

      break;
    }

    case ExpressionTypeFunctionExpression: {
      FunctionExpression *function_expression = (FunctionExpression *)expression;

      printf("FUNCTION");

      if (function_expression->identifier) printf_expression(function_expression->identifier, alignment);
      printf_expression(function_expression->arguments, alignment);
      printf("\n");
      printf_block(function_expression->block, alignment);

      break;
    }

    case ExpressionTypeBoolExpression: {
      printf(" %s ", ((BoolLiteral *)expression->literal)->bool_literal ? "true" : "false");

      break;
    }

    case ExpressionTypeIntegerExpression: {
      printf(" %lu ", ((IntegerLiteral *)expression->literal)->integer_literal);

      break;
    }

    case ExpressionTypeFloatExpression: {
      printf(" %f ", ((FloatLiteral *)expression->literal)->float_literal);

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

    default: {

    }
  }
}


void printf_statement(Statement *statement, unsigned int alignment) {
  switch (statement->statement_type) {
    case StatementTypePrintStatement: {
      PrintStatement *print_statement = (PrintStatement *)statement;

      printf_alignment(alignment);
      printf("print ");
      printf_expression(print_statement->right_expression, alignment);
      printf("\n");

      break;
    }

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
}


void free_expression(Expression *expression) {
  switch (expression->expression_type) {
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

    case ExpressionTypePostfixExpression: {
      PostfixExpression *postfix_expression = (PostfixExpression *)expression;

      free_expression(postfix_expression->left_expression);
      free(postfix_expression);

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

    case ExpressionTypeMemberExpression: {
      MemberExpression *member_expression = (MemberExpression *)expression;

      for (size_t i = 0; i < member_expression->expression_count; i++) {
        free_expression(member_expression->expressions[i]);
      }

      free(member_expression->expressions);
      free(member_expression);

      break;
    }

    case ExpressionTypeFunctionExpression: {
      FunctionExpression *function_expression = (FunctionExpression *)expression;

      if (function_expression->identifier) free_expression(function_expression->identifier);
      free_expression(function_expression->arguments);
      free_block(function_expression->block);
      free(function_expression);

      break;
    }

    case ExpressionTypeIntegerExpression: {
      free(expression->literal);
      free(expression);
      break;
    }

    case ExpressionTypeFloatExpression: {
      free(expression->literal);
      free(expression);
      break;
    }

    case ExpressionTypeStringExpression: {
      free(expression->literal);
      free(expression);
      break;
    }

    case ExpressionTypeIdentifierExpression: {
      free(expression->literal);
      free(expression);
      break;
    }

    default: {

    }
  }
}


void free_block(Block *block) {
  for (size_t i = 0; i < block->statement_count; i++) {
    free_statement(block->statements[i]);
  }
  free(block);
}


void free_block_definition(BlockDefinition *block_definition) {
  switch (block_definition->block_definition_type) {
    case BlockDefinitionTypeIfBlock: {
      IfBlockDefinition *if_block_definition = (IfBlockDefinition *)block_definition;

      free_block(if_block_definition->block);
      free_expression(if_block_definition->condition);
      if (if_block_definition->pre_expression) free_expression(if_block_definition->pre_expression);
      free(block_definition);

      break;
    }

    case BlockDefinitionTypeForBlock: {
      ForBlockDefinition *for_block_definition = (ForBlockDefinition *)block_definition;

      free_block(for_block_definition->block);
      free_expression(for_block_definition->condition);
      if (for_block_definition->pre_expression) free_expression(for_block_definition->pre_expression);
      if (for_block_definition->post_expression) free_expression(for_block_definition->post_expression);
      free(block_definition);

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

    case StatementTypePrintStatement: {
      PrintStatement *print_statement = (PrintStatement *)statement;

      free_expression(print_statement->right_expression);
      free(print_statement);
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

    default: {

    }
  }
}


void free_ast(AST *ast) {
  free_block(ast->block);
  free(ast);
}


Operator token_to_operator(Token token) {
  switch (token.token_type) {
    case MEMBER_TOKEN:
      return MEMBER_OP;

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

    case PLUS_PLUS_TOKEN:
      return PLUS_PLUS_OP;

    case MINUS_MINUS_TOKEN:
      return MINUS_MINUS_OP;

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


bool check_if_token_is_postfix_operator(Token token) {
  Operator operator = token_to_operator(token);

  return operator == PLUS_PLUS_OP || operator == MINUS_MINUS_OP;
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
    };

    case MULTIPLICATION_OP:
    case DIVISION_OP:
    case INTEGER_DIVISION_OP:
    case MOD_OP: {
      result = MULTIPLICATION_DIVISION_MOD_PRECEDENCE;
      break;
    };

    case EXPONENT_OP: {
      result = EXPONENT_PRECEDENCE;
      break;
    };

    case PLUS_PLUS_OP:
    case MINUS_MINUS_OP: {
      result = PLUS_PLUS_MINUS_MINUS_PRECEDENCE;
      break;
    };

    case L_PARENTHESIS_OP: {
      result = CALL_PRECEDENCE;
      break;
    };

    case L_BRACKET_OP: {
      result = LIST_PRECEDENCE;
      break;
    };

    case MEMBER_OP: {
      result = MEMBER_PRECEDENCE;
      break;
    };

    case IN_OP: {
      result = IN_OP_PRECEDENCE;
      break;
    }

    case COMMA_OP: {
      result = COMMA_PRECEDENCE;
      break;
    };

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
