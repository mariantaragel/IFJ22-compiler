/****
 ** codegen.h
 ** Řešení IFJ-PROJEKT, 11.11.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file codegen.h
 * @author xhorva17
 * @brief Code generation interface.
 * @date 2022-11-11
 */

#include <stdio.h>
#include <stdlib.h>
#include "dynamic_string.h"


#ifndef CODEGEN_H
#define CODEGEN_H

// typedef enum {VAR_DEF, FUNC_DEF, FUNC_LABEL, WHILE, IF} label_type;

/**
 * @brief Prints instruction onto the standard output.
 * @param ins Instruction to print.
*/
// void ins_out(const char * ins);

/**
 * @brief Generate a unique label given
*/
// char * gen_label(char * label, label_type type);

/**
 * @brief Generates a flag for each variable in the token array.
 * @param tarr Expression to check.
*/
// void var_check(token_array_t * tarr);


/**
 * 
*/
// void ins_build(dynamic_string_t * ds);

#endif // CODEGEN_H