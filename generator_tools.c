/****
 ** labelgen.c
 ** Řešení IFJ-PROJEKT, 11.11.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file labelgen.c
 * @author xhorva17
 * @brief Code generation implementation.
 * @date 2022-11-11
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include "generator_tools.h"
#include "dynamic_string.h"
#include "token_array.h"

// int main() {
//     char * one, * two, * three;
//     for (int i = 0; i < 1000; i++) {
//         if(gen_if_labels(&one, &two, &three)) {
//             printf("%s\t%s\t%s\n\n", one, two, three);
//         }
//         if(gen_while_labels(&one, &two, &three)) {
//             printf("%s\t%s\t%s\n\n", one, two, three);
//         }
//     }
// }

unsigned ind = 0;

void inc_ind(){
	++ind;
	if(ind == 0) --ind;
}
void dec_ind(){
	if(ind != 0) --ind;
}

void G(const char* fmt, ...){
    va_list arg_list;
    va_start(arg_list, fmt);

	// print indentation
	for(unsigned i = 0; i < ind; ++i){
		printf("    "); // print 4 spaces
	}

	// print message
    vfprintf(stdout, fmt, arg_list);
	printf("\n");

    va_end(arg_list);
}

char * gen_label(char * prefix, char * infix, char * suffix, bool gen_num) {
    static unsigned num = 1;
    dynamic_string_t * ds;
    if(prefix != NULL) { ds = ds_strinit(prefix); } else { ds = ds_init(); }
    if(infix != NULL) { ds_concat_str(ds, infix); }
    if(suffix != NULL) { ds_concat_str(ds, suffix); }
    if(gen_num) {
        ds_write(ds, '_');
        ds_write_uint(ds, num);
        num++;
    }
    return ds_get_str(ds, true); // Return string and delete ds.
}

bool gen_while_labels(char ** start, char ** body_start, char ** end) {
    static unsigned while_count = 1; // Number of while loops generated in the program.
    dynamic_string_t * ds_start = ds_strinit("?while_start_");
    dynamic_string_t * ds_body_start = ds_strinit("?while_body_start_");
    dynamic_string_t * ds_end = ds_strinit("?while_end_");

    if(ds_start == NULL || ds_body_start == NULL || ds_end == NULL) {
        ds_dstr(ds_start);
        ds_dstr(ds_body_start);
        ds_dstr(ds_end);
        return false;
    }
    if(ds_write_uint(ds_start, while_count) || ds_write_uint(ds_body_start, while_count) || ds_write_uint(ds_end, while_count)) {
        ds_dstr(ds_start);
        ds_dstr(ds_body_start);
        ds_dstr(ds_end);
        return false;
    }
    if(!(*start = ds_get_str(ds_start, true)) || !(*body_start = ds_get_str(ds_body_start, true)) || !(*end = ds_get_str(ds_end, true))) {
        free(*start);
        free(*body_start);
        free(*end);
        ds_dstr(ds_start);
        ds_dstr(ds_body_start);
        ds_dstr(ds_end);
        return false;
    }
    while_count++;
    return true;
}

bool gen_if_labels(char ** branch_true, char ** branch_false, char ** end) {
    static unsigned if_count = 1; // Number of while loops generated in the program.
    dynamic_string_t * ds_branch_true = ds_strinit("?if_branch_true_");
    dynamic_string_t * ds_branch_false = ds_strinit("?if_branch_false_");
    dynamic_string_t * ds_end = ds_strinit("?if_end_");
    
    if(ds_branch_true == NULL || ds_branch_false == NULL || ds_end == NULL) {
        ds_dstr(ds_branch_true);
        ds_dstr(ds_branch_false);
        ds_dstr(ds_end);
        return false;
    }
    if(ds_write_uint(ds_branch_true, if_count) || ds_write_uint(ds_branch_false, if_count) || ds_write_uint(ds_end, if_count)) {
        ds_dstr(ds_branch_true);
        ds_dstr(ds_branch_false);
        ds_dstr(ds_end);
        return false;
    }
    if(!(*branch_true = ds_get_str(ds_branch_true, true)) || !(*branch_false = ds_get_str(ds_branch_false, true)) || !(*end = ds_get_str(ds_end, true))) {
        free(*branch_true);
        free(*branch_false);
        free(*end);
        ds_dstr(ds_branch_true);
        ds_dstr(ds_branch_false);
        ds_dstr(ds_end);
        return false;
    }
    if_count++;
    return true;
}


bool gen_pop_labels(char ** type_ok, char ** skip_nil_conversion) {
    static unsigned pop_count = 1;
    dynamic_string_t * ds_type_ok = ds_strinit("?type_ok");
    dynamic_string_t * ds_skip_nil_conversion = ds_strinit("?skip_nil_conversion");

    if(ds_type_ok == NULL || ds_skip_nil_conversion == NULL) {
        ds_dstr(ds_type_ok);
        ds_dstr(ds_skip_nil_conversion);
        return false;
    }
    if(ds_write_uint(ds_type_ok, pop_count) || ds_write_uint(ds_skip_nil_conversion, pop_count)) {
        ds_dstr(ds_type_ok);
        ds_dstr(ds_skip_nil_conversion);
        return false;
    }
    if(!(*type_ok = ds_get_str(ds_type_ok, true)) || !(*skip_nil_conversion = ds_get_str(ds_skip_nil_conversion, true))) {
        free(*type_ok);
        free(*skip_nil_conversion);
        ds_dstr(ds_type_ok);
        ds_dstr(ds_skip_nil_conversion);
        return false;
    }
    pop_count++;
    return true;
}
