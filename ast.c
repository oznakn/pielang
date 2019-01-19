#include "ast.h"

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"

void print_expression(Expression *expression) {
  if (expression->expression_type == ExpressionTypeInfixExpression) {
    InfixExpression *infix_expression = (InfixExpression *)expression;

    printf(" (");
    print_expression(infix_expression->left_expression);

    if (infix_expression->operator == ADDITION_OP) { printf(" + "); }
    else if (infix_expression->operator == SUBTRACTION_OP) { printf(" - "); }
    else if (infix_expression->operator == MULTIPLICATION_OP) { printf(" * "); }
    else if (infix_expression->operator == DIVISION_OP) { printf(" / "); }
    else if (infix_expression->operator == MOD_OP) { printf(" %% "); }
    else if (infix_expression->operator == EXPONENT_OP) printf(" ^ ");

    print_expression(infix_expression->right_expression);
    printf(") ");
  } else if (expression->expression_type == ExpressionTypePrefixExpression) {
    PrefixExpression *prefix_expression = (PrefixExpression *)expression;

    printf(" (");

    if (prefix_expression->operator == ADDITION_OP) { printf(" +"); }
    else if (prefix_expression->operator == SUBTRACTION_OP) { printf(" -"); }
    else if (prefix_expression->operator == PLUS_PLUS_OP) { printf("++"); }
    else if (prefix_expression->operator == MINUS_MINUS_OP) { printf("--"); }

    print_expression(prefix_expression->right_expression);
    printf(") ");

  } else if (expression->expression_type == ExpressionTypePostfixExpression) {
    PostfixExpression *postfixExpression = (PostfixExpression *)expression;

    printf(" (");
    print_expression(postfixExpression->left_expression);

    if (postfixExpression->operator == PLUS_PLUS_OP) { printf("++"); }
    else if (postfixExpression->operator == MINUS_MINUS_OP) { printf("--"); }

    printf(") ");

  } else if (expression->expression_type == ExpressionTypeNumberExpression) {
    printf(" %f ", expression->value.number_value);
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

    case ExpressionTypeNumberExpression:
    case ExpressionTypeStringExpression: {
      free(expression->value.string_value);
      free(expression);
      break;
    }

    default: {

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

    default: {

    }
  }
}

void free_ast(AST *ast) {
  for (size_t i = 0; i < ast->statement_count; i++) {
    free_statement(ast->statements[i]);
  }
  free(ast);
}

/*
 *
  public
  static final int ASSIGNMENT = 1;
  public
  static final int CONDITIONAL = 2;
  public
  static final int SUM = 3;
  public
  static final int PRODUCT = 4;
  public
  static final int EXPONENT = 5;
  public
  static final int PREFIX = 6;
  public
  static final int POSTFIX = 7;
  public
  static final int CALL = 8;
  */

unsigned short get_precedence(Token token, unsigned short curr_precedence, bool b) {
  if (curr_precedence == 5 && token.token_type == EXPONENT_TOKEN) {
    if (b) { return 6; }
    else { return 4; }
  }

  switch (token.token_type) {
    case ADDITION_TOKEN:
    case SUBTRACTION_TOKEN:
      return 3;

    case MULTIPLICATION_TOKEN:
    case DIVISION_TOKEN:
    case INTEGER_DIVISION_TOKEN:
    case MOD_TOKEN:
      return 4;

    case EXPONENT_TOKEN:
      return 5;

    default:
      return 0;
  }
}

bool is_right_associative(Token token) {
  if (token.token_type == EXPONENT_TOKEN) return true;

  return false;
}

Expression *eval_token(Token token) {
  Expression *expression = NULL;

  if (token.token_type == NUMBER_TOKEN) {
    expression = malloc(sizeof(Expression));

    expression->value = (Value){.number_value = strtof(token.content, NULL)};
    expression->expression_type = ExpressionTypeNumberExpression;

    return expression;
  } else if (token.token_type == STRING_LITERAL_TOKEN) {
    expression = malloc(sizeof(Expression));

    expression->value = (Value){.string_value = token.content};
    expression->expression_type = ExpressionTypeNumberExpression;

    return expression;
  }

  return expression;
}

// http://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/
Expression *parse_expression(Lexer *lexer, unsigned short precedence, TokenType until) {
  Token curr_token = next_token(lexer);

  Expression *left;

  if (check_if_token_is_operator(curr_token)) {
    PrefixExpression *prefix_expression = malloc(sizeof(PrefixExpression));

    prefix_expression->expression = (Expression){.expression_type = ExpressionTypePrefixExpression};
    prefix_expression->operator = token_to_operator(curr_token);
    prefix_expression->right_expression = eval_token(next_token(lexer));

    left = (Expression *)prefix_expression;
  } else {
    left = eval_token(curr_token);
  }

  curr_token = peek_token(lexer);

  while (precedence < get_precedence(curr_token, precedence, true)) {
    curr_token = next_token(lexer);

    if (curr_token.token_type == until || curr_token.token_type == EOF_TOKEN) return left; //TODO

    /* if (peek_token(lexer).token_type == PLUS_PLUS_TOKEN) {
      PostfixExpression *postfix_expression = malloc(sizeof(PostfixExpression));

      postfix_expression->expression = (Expression){.expression_type = ExpressionTypePostfixExpression};
      postfix_expression->operator = token_to_operator(next_token(lexer));
      postfix_expression->left_expression = left;

      left = (Expression *)postfix_expression;

      curr_token = next_token(lexer);

      if (curr_token.token_type == until || curr_token.token_type == EOF_TOKEN) return left; //TODO
    } */

    InfixExpression *infix_expression = malloc(sizeof(InfixExpression));

    infix_expression->expression = (Expression){.expression_type = ExpressionTypeInfixExpression};
    infix_expression->left_expression = left;
    infix_expression->operator = token_to_operator(curr_token);
    infix_expression->right_expression = parse_expression(lexer, get_precedence(curr_token, precedence, false), until);

    left = (Expression *)infix_expression;
  }

  return left;
}

AST *parse(Lexer *lexer) {
  AST *ast = malloc(sizeof(AST));
  ast->statement_count = 0;

  ast->statements = malloc(ast->statement_count);
  Statement **tmp = NULL;

  while (true) {
    Token token = peek_token(lexer);

    if (token.token_type == EOF_TOKEN) { break; }

    tmp = realloc(ast->statements, (++ast->statement_count) * sizeof(Statement *));
    if (tmp == NULL) {
      free(ast->statements);
      break;
    }
    ast->statements = tmp;

    if (true) {
      Expression *expression = parse_expression(lexer, 0, EOL_TOKEN);

      ExpressionStatement *expression_statement = malloc(sizeof(ExpressionStatement));
      expression_statement->statement = (Statement){.statement_type = StatementTypeExpressionStatement};
      expression_statement->expression = expression;

      print_expression(expression);

      ast->statements[ast->statement_count - 1] = (Statement *)expression_statement;
    }
  }

  return ast;
}


