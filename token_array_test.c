#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token_array.h"

int main(){
    token_array_t* token_array1 = token_array_create();
	if(token_array1 == NULL) return -1;

	token_t* ta[10];
	for(size_t i = 0; i < 10; i++){
		ta[i] = t_init();
	}

	for(size_t i = 0; i < 10; i++){
		if(ta[i] == NULL){
			for(size_t k = 0; k < 10; k++){
				t_dstr(ta[k]);
			}
			token_array_free(token_array1);
			return -1;
		}
	}

	token_array_push_token(token_array1, ta[0]);
	token_array_pop_token(token_array1);
	token_array_push_token(token_array1, ta[1]);
	token_array_push_token(token_array1, ta[2]);
	token_array_pop_token(token_array1);
	token_array_push_token(token_array1, ta[3]);
	token_array_pop_token(token_array1);
	token_array_pop_token(token_array1);
	token_array_pop_token(token_array1);
	token_array_push_token(token_array1, ta[4]);
	token_array_push_token(token_array1, ta[5]);
	token_array_push_token(token_array1, ta[6]);

	token_array_t* token_array2 = token_array_create();
	if(token_array2 == NULL){
		for(size_t k = 0; k < 10; k++){
			t_dstr(ta[k]);
		}
		token_array_free(token_array1);
		return -1;
	}

	token_array_push_token(token_array2, ta[7]);
	token_array_pop_token(token_array2);
	token_array_push_token(token_array1, ta[8]);
	token_array_push_token(token_array1, ta[9]);
	
	if(token_array_append_tokens(token_array2, token_array1) != 0) return -1;

	token_array_free(token_array2);

	return 0;
}