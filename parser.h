/**
 * @name parser.h
 * @brief Interface of top-down parser
 * @authors Marián Tarageľ
 * @date 20.10.2022
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"

/**
 * @brief Checks syntax of a program
 * 
 * @return 0 if syntax is correct, otherwise error code
 */
void program();

/**
 * @brief Checks syntax of a prolog
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void php_start(token_t *token);

/**
 * @brief Chesks syntax of a program body
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void program_body(token_t *token);

/**
 * @brief Checks syntax of a program ending
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void php_end(token_t *token);

/**
 * @brief Checks syntax of a function definition
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void func_def(token_t *token);

/**
 * @brief Checks syntax of a statement list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void stmt_list(token_t *token);

/**
 * @brief Checks syntax of a parameter list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void param_list(token_t *token);

/**
 * @brief Checks syntax of a return type
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void return_type(token_t *token);

/**
 * @brief Checks syntax of a types
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void type(token_t *token);

/**
 * @brief Checks syntax of a next parameters
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void param_next(token_t *token);

/**
 * @brief Checks syntax of a statement list ending
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void stmt_list_bracket_end(token_t *token);

/**
 * @brief Checks syntax of a statement
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void stmt(token_t *token);

/**
 * @brief Checks syntax of a function call
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code 
 */
void func_call(token_t *token);

/**
 * @brief Checks syntax of a if statement
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void if_stmt(token_t *token);

/**
 * @brief Checks syntax of a while statement
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void while_stmt(token_t *token);

/**
 * @brief Checks syntax of an argument list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void arg_list(token_t *token);

/**
 * @brief Checks syntax of a next agrument
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void arg_next(token_t *token);

/**
 * @brief Checks syntax of an argument
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @return 0 if syntax is correct, otherwise error code
 */
void arg(token_t *token);

#endif