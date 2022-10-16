/**
 * @name parser.h
 * @brief Interface of top-down parser
 * @authors Marián Tarageľ
 * @date 16.10.2022
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define TRUE 1
#define FALSE 0

typedef enum error {SYNTAX_CORRECT, LEXICAL_ERROR, SYNTAX_ERROR, SEMANTIC_ERROR} return_t;

/**
 * @brief Checks syntax of a program
 * 
 * @return 0 if syntax is correct, otherwise error code
 */
int program();

/**
 * @brief Checks syntax of a prolog
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int php_start(token_t *token);

/**
 * @brief Chesks syntax of a program body
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int program_body(token_t *token);

/**
 * @brief Checks syntax of a program ending
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int php_end(token_t *token);

/**
 * @brief Checks syntax of a function definition
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int func_def(token_t *token);

/**
 * @brief Checks syntax of a statement list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int stmt_list(token_t *token);

/**
 * @brief Checks syntax of a parameter list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int param_list(token_t *token);

/**
 * @brief Checks syntax of a return type
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int return_type(token_t *token);

/**
 * @brief Checks syntax of a types
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int type(token_t *token);

/**
 * @brief Checks syntax of a next parameters
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int param_next(token_t *token);

/**
 * @brief Checks syntax of a statement list ending
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int stmt_list_bracket_end(token_t *token);

/**
 * @brief Checks syntax of a statement
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
int stmt(token_t *token)

#endif