/**
 * @name parser.h
 * @brief Interface of top-down parser
 * @authors Marián Tarageľ
 * @date 15.10.2022
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define TRUE 1
#define FALSE 0

typedef enum error {SYNTAX_CORRECT, LEXICAL_ERROR, SYNTAX_ERROR, SEMANTIC_ERROR} return_t;

int program();

int php_start();

int program_body();

int php_end();

int func_def();

int stmt_list();

int param_list();

int return_type(token_t *token);

int type(token_t *token);

int param_next();

int stmt_list_bracket_end();

#endif