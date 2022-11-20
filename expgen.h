/****
 ** expgen.h
 ** Řešení IFJ-PROJEKT, 13.11.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file expgen.h
 * @author xhorva17
 * @brief Expression generation.
 * @date 2022-13-11
 */

#include "token_array.h"
#include <stdbool.h>
#include "generator.h"
#include "token.h"

#ifndef EXPGEN_H
#define EXPGEN_H


typedef enum {RELATIONAL, ARITHMETIC, DIVISION, STRING, EQUALITY} operation_t;

void gen_expression(token_array_t * tarr, generator_context_t * gc);
void push_operand(token_t * token, const char * scope_label);
void set_operator(operation_t * operation, token_t * token);
void pop_operand(operation_t operation, const char * operand, const char * operand_type);
void check_types(operation_t operation, char * execute_label);
void execute_operation(operation_t operation, token_type_t operator, char * execute_label);
bool isoperand(token_t * token);
bool isoperator(token_t * token);

// TESTING ONLY
token_array_t * get_exp(char* expression);


#endif // EXPGEN_H