/****
 ** error.c
 ** Řešení IFJ-PROJEKT, 20.10.2022
 ** Autor: xhorva17, xtarag01
 ** Přeloženo:
 **/

/**
 * @file error.c
 * @author xhorva17, xtarag01
 * @brief Error function implementation.
 * @date 2022-10-20
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

error_codes_t error;

void error_print(error_codes_t error) {
    if(error == OK) {
        fprintf(stderr, "ERROR: %d OK\n", OK);
    } else if (error == LEXICAL_ERROR) {
        fprintf(stderr, "ERROR: %d LEXICAL_ERROR\n", LEXICAL_ERROR);
    } else if (error == SYNTAX_ERROR) {
        fprintf(stderr, "ERROR: %d SYNTAX_ERROR\n", SYNTAX_ERROR);
    } else if (error == SEM_ERROR_3) {
        fprintf(stderr, "ERROR: %d SEM_ERROR_3\n", SEM_ERROR_3);
    } else if (error == SEM_ERROR_4) {
        fprintf(stderr, "ERROR: %d SEM_ERROR_4\n", SEM_ERROR_4);
    } else if (error == SEM_ERROR_5) {
        fprintf(stderr, "ERROR: %d SEM_ERROR_5\n", SEM_ERROR_5);
    } else if (error == SEM_ERROR_6) {
        fprintf(stderr, "ERROR: %d SEM_ERROR_6\n", SEM_ERROR_6);
    } else if (error == SEM_ERROR_7) {
        fprintf(stderr, "ERROR: %d SEM_ERROR_7\n", SEM_ERROR_7);
    } else if (error == SEM_ERROR_8) {
        fprintf(stderr, "ERROR: %d SEM_ERROR_8\n", SEM_ERROR_8);
    } else if (error == INTERNAL_ERROR) {
        fprintf(stderr, "ERROR: %d INTERNAL_ERROR\n", INTERNAL_ERROR);
    }
}
