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

/**
 * @brief Generates IFJcode22 code which calculates the result of an expression. Function assumes that that 'tarr'
 * is a valid postfix expression, if not, behaviour is undefined.
 * @param tarr Expression (in postfix notation) to generate.
 * @param gc Program context (local/global). 
*/
void gen_expression(token_array_t * tarr, generator_context_t * gc);

/**
 * @brief Generates IFJcode22 code of function definitions, for all (arithmetic, string, relational) operations.
*/
void gen_operation_functions();

#endif // EXPGEN_H