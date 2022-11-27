/**
 * @file generator_tools.h
 * @author Martin Horvát (xhorva17@stud.fit.vutbr.cz), David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Code generation tools iterface.
 * @date 2022-11-26
 * 
 */


#ifndef GENERATOR_TOOLS_H
#define GENERATOR_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct{
    bool is_in_function;
	char* func_end_label;
}generator_context_t;

extern unsigned ind;

void inc_ind();

void dec_ind();

void G(const char* fmt, ...);

char * gen_label(char * prefix, char * infix, char * suffix, bool gen_num);

bool gen_while_labels(char ** start, char ** body_start, char ** end);

bool gen_if_labels(char ** branch_true, char ** branch_false, char ** end);

bool gen_pop_labels(char ** type_ok, char ** skip_nil_conversion);

#endif // LABELGEN_H