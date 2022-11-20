/****
 ** labelgen.h
 ** Řešení IFJ-PROJEKT, 11.11.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file labelgen.h
 * @author xhorva17
 * @brief Code generation interface.
 * @date 2022-11-11
 */

#ifndef GENERATOR_TOOLS_H
#define GENERATOR_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern size_t ind;

void inc_ind();

void dec_ind();

void G(const char* fmt, ...);

char * gen_label(char * prefix, char * infix, char * suffix, bool gen_num);

bool gen_while_labels(char ** start, char ** body_start, char ** end);

bool gen_if_labels(char ** branch_true, char ** branch_false, char ** end);

#endif // LABELGEN_H