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


char * gen_label(char * prefix, char * infix, char * suffix, bool gen_num);

char * inttostr(size_t n);

#endif // CODEGEN_H