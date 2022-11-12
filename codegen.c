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
    char * str = gen_label("$", "var_name", NULL, true);
    printf("%s\n",str);


    printf("%s\n", inttostr(1205));

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

char * inttostr(size_t n) {
    size_t size = n;
    if(n == 0) {
        size++;
    }
	char * str = (char*) malloc((int)((ceil(log10(size))+2)*sizeof(char)));
    if(str == NULL) {
        return NULL;
    }
	sprintf(str, "%ld", n);
    return str; 
}
