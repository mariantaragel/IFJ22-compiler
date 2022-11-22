#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token_array.h"

void test_pop(token_array_t* token_array){
	size_t prev_size = token_array->token_count;
	token_t* popped_token = token_array_pop_token(token_array);
	if(prev_size == 0){
		if(popped_token != NULL) exit(-1);
	}
	else{
		if(popped_token == NULL || token_array->token_count != prev_size-1) exit(-1);
		t_dstr(popped_token);
	}
}

void test_push(token_array_t* token_array, token_t* token){
	size_t prev_size = token_array->token_count;
	if(token_array_push_token(token_array, token) != 0) exit(-1);
	if(token_array->token_count != prev_size+1) exit(-1);
	if(token_array->array[token_array->token_count-1] != token) exit(-1);
}

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

	token_array_reverse(token_array1);
	test_push(token_array1, ta[0]);
	token_array_reverse(token_array1);
	test_pop(token_array1);
	test_pop(token_array1);
	test_push(token_array1, ta[1]);
	test_push(token_array1, ta[2]);
	token_array_reverse(token_array1);
	test_pop(token_array1);
	test_pop(token_array1);
	test_push(token_array1, ta[3]);
	test_pop(token_array1);
	test_pop(token_array1);
	test_pop(token_array1);
	test_pop(token_array1);
	test_push(token_array1, ta[4]);
	test_push(token_array1, ta[5]);
	test_push(token_array1, ta[6]);
	token_array_reverse(token_array1);

	token_array_t* token_array2 = token_array_create();
	if(token_array2 == NULL){
		for(size_t k = 0; k < 10; k++){
			t_dstr(ta[k]);
		}
		token_array_free(token_array1);
		return -1;
	}

	test_push(token_array2, ta[7]);
	test_pop(token_array2);
	test_push(token_array1, ta[8]);
	test_push(token_array1, ta[9]);
	
	if(token_array_append_tokens(token_array2, token_array1) != 0) return -1;

	token_array_free(token_array1);
	token_array_free(token_array2);

	return 0;
}