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
#include "codegen.h"
#include "token_array.h"

// int main() {
    
// }


char * gen_label(char * label, label_type ltype) {
    static unsigned if_labels = 0; // If, else, end -> (potentially) 3 labels required.
    static unsigned while_labels = 0; // While start + while end -> 2 labels required.


    // Load dynamic string ...  
    switch(ltype) {
        case VAR_DEF: break;
        case FUNC_DEF: break;
        case FUNC_LABEL: break;
        case WHILE: break;
        case IF: break;
    }
}
