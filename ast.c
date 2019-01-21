#include "ast.h"

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"

void printf_alignment(unsigned int alignment) {
  if (alignment != 0u) {
    for (unsigned int i = 0; i < alignment; i += 1u) {
      printf("    ");
    }
  }
}

void printf_expression(Expression *expression) {
  if (expression->expression_type == ExpressionTypeInfixExpression) {
    InfixExpression *infix_expression = (InfixExpression *)expression;

    printf(" (");
    printf_expression(infix_expression->left_expression);

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

    printf_expression(infix_expression->right_expression);
    printf(") ");
  } else if (expression->expression_type == ExpressionTypePrefixExpression) {
    PrefixExpression *prefix_expression = (PrefixExpression *)expression;

    printf(" (");

    if (prefix_expression->operator == NOT_OP) { printf("!"); }
    else if (prefix_expression->operator == ADDITION_OP) { printf("+"); }
    else if (prefix_expression->operator == SUBTRACTION_OP) { printf("-"); }
    else if (prefix_expression->operator == PLUS_PLUS_OP) { printf("++"); }
    else if (prefix_expression->operator == MINUS_MINUS_OP) { printf("--"); }

    printf_expression(prefix_expression->right_expression);
    printf(") ");

  } else if (expression->expression_type == ExpressionTypePostfixExpression) {
    PostfixExpression *postfix_expression = (PostfixExpression *)expression;

    printf(" (");
    printf_expression(postfix_expression->left_expression);

    if (postfix_expression->operator == PLUS_PLUS_OP) { printf("++"); }
    else if (postfix_expression->operator == MINUS_MINUS_OP) { printf("--"); }

    printf(") ");

  } else if (expression->expression_type == ExpressionTypeCallExpression) {
    CallExpression *call_expression = (CallExpression *)expression;

    printf(" `");
    printf_expression(call_expression->identifier_expression);
    printf("->");
    printf_expression(call_expression->tuple_expression);
    printf("` ");
  } else if (expression->expression_type == ExpressionTypeTupleExpression) {
    TupleExpression *tuple_expression = (TupleExpression *)expression;

    printf(" T(");
    for (size_t i = 0; i < tuple_expression->expression_count; i++) {
      printf_expression(tuple_expression->expressions[i]);

      if (i < tuple_expression->expression_count - 1) {
        printf(",");
      }
    }
    printf(") ");
  } else if (expression->expression_type == ExpressionTypeIntegerExpression) {
    printf(" %lu ", expression->value.integer_value);
  } else if (expression->expression_type == ExpressionTypeFloatExpression) {
    printf(" %f ", expression->value.float_value);
  } else if (expression->expression_type == ExpressionTypeStringExpression) {
    printf(" \"%s\" ", expression->value.string_value);
  } else if (expression->expression_type == ExpressionTypeBoolExpression) {
    printf(" %s ", expression->value.bool_value ? "true" : "false");
  } else if (expression->expression_type == ExpressionTypeIdentifierExpression) {
    printf(" %s ", expression->value.string_value);
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
  if (block_definition->block_definition_type == BlockDefinitionTypeIfBlock) {
    IfBlockDefinition *if_block_definition = (IfBlockDefinition *)block_definition;

    printf_alignment(alignment);
    printf("IF");
    printf_expression(if_block_definition->condition);
    printf("\n");
    printf_block(if_block_definition->block_definition.block, alignment);
  }
}

void printf_statement(Statement *statement, unsigned int alignment) {
  if (statement->statement_type == StatementTypePrintStatement) {
    PrintStatement *print_statement = (PrintStatement *)statement;

    printf_alignment(alignment);
    printf("print ");
    printf_expression(print_statement->right_expression);
    printf("\n");
  } else if (statement->statement_type == StatementTypeReturnStatement) {
    ReturnStatement *return_statement = (ReturnStatement *)statement;

    printf_alignment(alignment);
    printf("return ");
    printf_expression(return_statement->right_expression);
    printf("\n");
  } else if (statement->statement_type == StatementTypeImportStatement) {
    ImportStatement *import_statement = (ImportStatement *)statement;

    printf_alignment(alignment);
    printf("import ");
    printf_expression(import_statement->right_expression);
    printf("\n");
  } else if (statement->statement_type == StatementTypeBlockDefinitionStatement) {
    BlockDefinitionStatement *block_definition_statement = (BlockDefinitionStatement *)statement;

    printf_block_definition(block_definition_statement->block_definition, alignment);
  } else if (statement->statement_type == StatementTypeExpressionStatement) {
    ExpressionStatement *expression_Statement = (ExpressionStatement *)statement;

    printf_alignment(alignment);
    printf_expression(expression_Statement->expression);
    printf("\n");
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

    case ExpressionTypeTupleExpression: {
      TupleExpression *tuple_expression = (TupleExpression *)expression;

      for (size_t i = 0; i < tuple_expression->expression_count; i++) {
        free_expression(tuple_expression->expressions[i]);
      }

      free(tuple_expression->expressions);
      free(tuple_expression);

      break;
    }

    case ExpressionTypeIntegerExpression: {
      free(expression);
      break;
    }

    case ExpressionTypeFloatExpression: {
      free(expression);
      break;
    }

    case ExpressionTypeStringExpression: {
      free(expression->value.string_value);
      free(expression);
      break;
    }

    case ExpressionTypeIdentifierExpression: {
      free(expression->value.string_value);
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
  if (block_definition->block_definition_type == BlockDefinitionTypeIfBlock) {
    IfBlockDefinition *if_block_definition = (IfBlockDefinition *)block_definition;

    free_block(if_block_definition->block_definition.block);
    free_expression(if_block_definition->condition);
    free(block_definition);
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

    case EXCLAMATION_TOKEN:
      return NOT_OP;

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
      result = PREFIX_PRECEDENCE;
      break;
    };

    case L_PARENTHESIS_OP: {
      result = L_PARENTHESIS_PRECEDENCE;
      break;
    };

    case MEMBER_OP: {
      result = MEMBER_PRECEDENCE;
      break;
    };

    default: {
      result = 0;
    }
  }

  if (next && is_operator_right_associative(operator)) result -= 1u;

  return result;
}
