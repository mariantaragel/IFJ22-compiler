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

void gen_expression(token_array_t * tarr, generator_context_t * gc);
void gen_operation_functions();

#endif // EXPGEN_H