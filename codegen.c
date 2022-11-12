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
#include <math.h>
#include "codegen.h"
#include "token_array.h"

/*
int main() {
    dynamic_string_t * ds = ds_strinit("I am counting: ");
    dynamic_string_t * ds2 = ds_strinit("New strinero");

    ds_concat(ds, ds2);

    int n = 12;
    while(n < 50) {
        // printf("%d\n", (int)ceil(log10(n)));
        ds_write_uint(ds, n);
        ds_write(ds, ',');
        ds_write(ds, ' ');
        n++;
    }
    printf("%s\n", ds->str);
}
*/

// Get label type and generate label by concatenating strings...
// char * gen_label(char * label, label_type ltype) {
//     static unsigned if_labels = 0; // If, else, end -> (potentially) 3 labels required.
//     static unsigned while_labels = 0; // While start + while end -> 2 labels required.


//     // Load dynamic string ...  
//     switch(ltype) {
//         case VAR_DEF: break;
//         case FUNC_DEF: break;
//         case FUNC_LABEL: break;
//         case WHILE: break;
//         case IF: break;
//     }
// }
