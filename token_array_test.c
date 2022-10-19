#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token_array.h"

int main(){
    token_array_t* token_array = token_array_create();
	if(token_array == NULL) return -1;

	token_array_free(token_array);
	return 0;
}