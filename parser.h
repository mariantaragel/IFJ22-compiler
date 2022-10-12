/**
 * @name parser.h
 * @brief Interface of top-down parser
 * @authors Marián Tarageľ
 * @date 12.10.2022
 */

#ifndef _PARSER_H
#define _PARSER_H

#include "token.h"

#define TRUE 1
#define FALSE 0
#define SYNTAX_CORRECT 1
#define SYNTAX_ERROR 0

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

#endif