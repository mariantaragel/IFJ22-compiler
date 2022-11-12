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
#include "codegen.h"
#include "token_array.h"

int main() {
    dynamic_string_t * ds = ds_strinit("Ahoj ");
    dynamic_string_t * ds2 = ds_strinit("svet");


    //printf("%s\n", ds->str);
    //printf("size: %ld\twi: %d\n", ds->size, ds->wi);
    while(ds->size < 200) {
        printf("%s\n", ds->str);
        printf("size: %ld\twi: %d\n", ds->size, ds->wi);
    
        //ds_write(ds, ' ');
        ds_concat(ds, ds2);
        ds_concat_str(ds, " pooper ");
        // ds_concat(ds2, ds);

    }
    ds_concat_str(ds, "hope for the best, prepare for the worst,->");
    ds_write(ds, 'O');
    ds_write(ds, 'K');
    ds_write(ds, '!');
            printf("%s\n", ds->str);
        printf("size: %ld\twi: %d\n", ds->size, ds->wi);

    //printf("%s\n", ds->str);
    //printf("%s\n", ds2->str);
    // ds3 = ds_strinit(ds->str);


    // ds_concat(ds, ds2);
    // ds_write(ds, ' ');
    // ds_write(ds, 'A');
    // ds_write(ds, 'H');
    // ds_write(ds, 'A');
    // ds_write(ds, 'H');
    // ds_write(ds, 'A');
    // while(ds->size < 300) {
    //     printf("%s\n%ld\n", ds->str, ds->size);    
    //     ds_concat(ds, ds3); 
    //     ds_write(ds, ' ');
    //     ds_write(ds, 'A');
    //     ds_write(ds, 'H');
    //     ds_write(ds, 'A');
    //     ds_write(ds, 'H');
    //     ds_write(ds, 'A');
    //     ds_write(ds, ' ');
    
    // }
    //ds_concat(ds3, ds);
    //ds_concat(ds3, ds2);
}


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
