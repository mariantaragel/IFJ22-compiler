/****
 ** codegen.c
 ** Řešení IFJ-PROJEKT, 11.11.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file codegen.c
 * @author xhorva17
 * @brief Code generation implementation.
 * @date 2022-11-11
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "codegen.h"
#include "token_array.h"

int main() {
    char * one, * two, * three;
    for (int i = 0; i < 30; i++) {
        gen_if_labels(&one, &two, &three);
        printf("%s\t%s\t%s\n\n", one, two, three);
        gen_while_labels(&one, &two, &three);
        printf("%s\t%s\t%s\n\n", one, two, three);
    }
}

char * gen_label(char * prefix, char * infix, char * suffix, bool gen_num) {
    static unsigned num = 1;
    dynamic_string_t * ds = ds_strinit(prefix);
    ds_concat_str(ds, infix);
    ds_concat_str(ds, suffix);
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
