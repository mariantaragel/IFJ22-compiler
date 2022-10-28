#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symtable.h"

void error_exit(char message[], symtable_t *symtable){
    printf("%s", message);
    symtable_free(symtable);
    exit(1);
}

int main(){
    
    char* test_names[] = {"Anapurna", "Baba", "Cecilie", "Dan", "E", "Fik"};

    const size_t name_count = (sizeof(test_names) / sizeof(*test_names));

    symtable_t* symtable = symtable_create();

    if(symtable_get_symbol_count(symtable) != 0){
        error_exit("err1", symtable);
    }

    symtable_clear(symtable);

    if(symtable_get_symbol_count(symtable) != 0){
        error_exit("err2", symtable);
    }

    symbol_info_t* symbol_info;


    bool name_found;

    for(size_t i = 0; i < name_count; ++i){
        symbol_info = symtable_lookup_insert(symtable, test_names[i], &name_found);

        if(symbol_info == NULL){
            error_exit("err3", symtable);
        }

        /*if(strcmp(symbol_info-, test_names[i]) != 0){
            error_exit("err4", symtable);
        }*/

        if(name_found != false){
            error_exit("err5", symtable);
        }

        if(symtable_get_symbol_count(symtable) != i+1){
            error_exit("err6", symtable);
        }

		/*
        for(size_t k = 0; k <= i; ++k){
            symbol_info = symtable_lookup(symtable, test_names[k]);
            if(symbol == NULL || strcmp(symbol->name, test_names[k]) != 0){
                error_exit("err7", symtable);
            }
        }
		*/

        for(size_t k = i+1; k < name_count; ++k){
            symbol_info = symtable_lookup(symtable, test_names[k]);
            if(symbol_info != NULL){
                error_exit("err8", symtable);
            }
        }
    }

    symtable_free(symtable);
}