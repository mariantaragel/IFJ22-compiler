/**
 * @name parser.h
 * @brief Interface of top-down parser
 * @authors Marián Tarageľ
 * @date 15.11.2022
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "abstract_syntax_tree.h"

#define RETURN_IF_ERROR if (error != OK) return
#define RETURN_ERROR(error_code) error = error_code; return
#define RETURN_ROOT if (error != OK) return root
#define RETURN_INTERNAL_ERROR(ptr) if (ptr == NULL) {RETURN_ERROR(INTERNAL_ERROR);}

#define TRUE 1
#define FALSE 0

/**
 * @brief Checks syntax of a program
 * 
 * @return On success abstract syntax tree root, otherwise null
 */
AST_node_t *program();

/**
 * @brief Checks syntax of a prolog
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 */
void php_start(token_t *token);

/**
 * @brief Chesks syntax of a program body
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void program_body(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a program ending
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 */
void php_end(token_t *token);

/**
 * @brief Checks syntax of a function definition
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void func_def(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a statement list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void stmt_list_bracket_start(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a parameter list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void param_list(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a return type
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void return_type(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a types
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void type(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a next parameters
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void param_next(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a statement list ending
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void stmt_list_bracket_end(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a statement
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void stmt(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a function call
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void func_call(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a if statement
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void if_stmt(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a while statement
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void while_stmt(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of an argument list
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void arg_list(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a next agrument
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void arg_next(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of an argument
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void arg(token_t *token, AST_node_t *parent);

/**
 * @brief Checks syntax of a function call assignment
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void func_call_assignment(token_t *token, AST_node_t *parent, token_t *func_id_token);

/**
 * @brief Checks syntax of a expression assignment
 * 
 * @param token one input symbol at each step, to make parsing action decision - LL(1) grammar
 * @param parent parent node in abstract syntax tree
 */
void exp_assignment(token_t *token, AST_node_t *parent, token_t *exp_token);

int is_token_type_correct(int num_of_types, token_t *token, ...);
void expression(token_t **token, int is_in_if_or_while, token_array_t *array);
void add_aval_to_node(token_t *token, AST_node_t *node);

#endif