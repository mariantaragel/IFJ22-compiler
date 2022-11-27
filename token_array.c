/**
 * @file token_array.c
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Token array implementation (declared in token_array.h).
 * @date 2022-11-26
 * 
 */

#include <stdlib.h> // NULL
#include <stdint.h> // SIZE_MAX
#include <stdio.h>

#include "token_array.h"
#include "token.h"

#define MAX_SIZE_TO_COUNT_RATIO 3

token_array_t* token_array_create(){
    // allocate token_array structure
    token_array_t* token_array = malloc(sizeof(*token_array));
    if(token_array == NULL) return NULL;

    // initialize structure
    token_array->size = 1;
    token_array->token_count = 0;

    // allocate token array to size 1
    token_array->array = malloc(sizeof(*(token_array->array)));

    // check for allocation error
    if(token_array->array == NULL){
        free(token_array);
        return NULL;
    }

    return token_array;
}

void token_array_free(token_array_t* token_array){
    if(token_array != NULL){
        // free all tokens inside token array
        for(size_t i = 0; i < token_array->token_count; ++i){
            t_dstr(token_array->array[i]);
        }

        // free token array
        free(token_array->array);

        // free token array structure itself
        free(token_array);
    }
}


/**
 * @brief Reallocates token array to new size.
 * 
 * @param token_array Token array to be reallocated.
 * @param new_size New size of token array.
 * @return int Returns 0 on success, otherwise non zero value is returned if reallocation fails.
 */
int _token_array_reallocate(token_array_t* token_array, size_t new_size){
	if(token_array->token_count > new_size) return -1;

    // store old token array, for when allocation fails
    token_t **tmp_array = token_array->array;

    // reallocate token array
    token_array->array = realloc(token_array->array, sizeof(token_t*) * new_size);

    // check if reallocation failed
    if(token_array->array == NULL){
        // reallocation failed, restore old array
        token_array->array = tmp_array;

        // return failure
        return -1;
    }

    // update size
    token_array->size = new_size;

    // return success
    return 1;
}

int token_array_push_token(token_array_t* token_array, token_t *token){
    if(token_array == NULL) return -1;

	// if token array is full, double its size
	if(token_array->token_count == token_array->size){

		// check for overflow, when doubling array size
		if(token_array->size > SIZE_MAX/2){
			return -1;
		}

		// new token array size
		size_t new_size = token_array->size * 2;

		// reallocate token array
		if(_token_array_reallocate(token_array, new_size) == -1){
			// return failure
			return -1;
		}
	}

	// add new token and update token count
	token_array->array[token_array->token_count] = token;
	(token_array->token_count)++;

	// return successs
	return 0;
}

token_t* token_array_pop_token(token_array_t* token_array){
	if(token_array == NULL) return NULL;
	if(token_array->token_count == 0) return NULL;

	// get topmost token
	token_t* popped_token = token_array->array[token_array->token_count-1];

	// update size
	--(token_array->token_count);

	// try to reallocate token array if token array size to token count exceeded MAX_SIZE_TO_COUNT_RATIO
	if(token_array->token_count != 0){
		if(token_array->size/token_array->token_count >= MAX_SIZE_TO_COUNT_RATIO){
			_token_array_reallocate(token_array, token_array->size / 2);
		}
	} 

	// return popped token
	return popped_token;
}


int token_array_append_tokens(token_array_t* token_array1, token_array_t* token_array2){
    // check for overflow, when adding together token counts of both token arrays
    if(token_array1->token_count > SIZE_MAX - token_array2->token_count){
        return -1;
    }

    // new token_array1 is sum of token counts of both token arrays
    size_t new_token_count = token_array1->token_count + token_array2->token_count;

    // double old token array size until new size is sufficient to hold all appended tokens
    size_t new_size = token_array1->size;
    while(new_size < new_token_count){
        if(new_size > SIZE_MAX/2){
            return -1;
        }
        new_size *= 2;
    }

    // reallocate token array to new size
    if(_token_array_reallocate(token_array1, new_size) == -1){
        return -1;
    }

    // move all tokens from token_array2 to token_array1
    for(size_t i = 0; i < token_array2->token_count; ++i){
        // add token from token_array2 to token_array1 and update token count
        token_array1->array[token_array1->token_count] = token_array2->array[i];
        (token_array1->token_count)++;
    }

    // set token count of second array to zero
    token_array2->token_count = 0;

	// return success
	return 0;
}

void token_array_reverse(token_array_t* token_array){
	if(token_array != NULL && token_array->token_count != 0){
		token_t* tmp;

		// left and right index of token_array
		// left index will move from left to right, right index will move from right to left
		size_t left = 0;
		size_t right = token_array->token_count - 1;

		// iterate until left index has not crossed with right index
		while(left < right){
			// swap left and right tokens
			tmp = token_array->array[left];
			token_array->array[left] = token_array->array[right];
			token_array->array[right] = tmp;

			// move left and right index
			++left;
			--right;
		}
	}
}

// print token as expression element
void _print_expr_token(token_t* t, FILE* fp) {
	if(t->type == VAR_ID) {
		fprintf(fp,"%s",t->aval);
	} else if(t->type == STR_LIT) {
		fprintf(fp,"str(%s)",t->aval);
	} else if(t->type == INT_LIT) {
		fprintf(fp,"int(%s)",t->aval);
	} else if(t->type == FLT_LIT) {
		fprintf(fp,"flt(%s)",t->aval);
	} else if(t->type == NULL_LIT) {
		fprintf(fp,"null");
	} else if(t->type == MUL) {
		fprintf(fp,"*");
	} else if(t->type == DIV) {
		fprintf(fp,"/");
	} else if(t->type == ADD) {
		fprintf(fp,"+");
	} else if(t->type == SUB) {
		fprintf(fp,"-");
	} else if(t->type == CONCAT) {
		fprintf(fp,".");
	} else if(t->type == LT) {
		fprintf(fp,"<");
	} else if(t->type == GT) {
		fprintf(fp,">");
	} else if(t->type == LTE) {
		fprintf(fp,"<=");
	} else if(t->type == GTE) {
		fprintf(fp,">=");
	} else if(t->type == EQ) {
		fprintf(fp,"===");
	} else if(t->type == NEQ) {
		fprintf(fp,"!==");
	} else if(t->type == LB) {
		fprintf(fp,"(");
	} else if(t->type == RB) {
		fprintf(fp,")");
	} else{
		fprintf(fp,"?(%d)",t->type);
	}
}

void token_array_expr_print(token_array_t* expr_token_array, FILE* fp){
	if(expr_token_array != NULL){
		for(size_t i = 0; i < expr_token_array->token_count; ++i){
			_print_expr_token(expr_token_array->array[i], fp);
		}
	}
	else{
		fprintf(fp,"NULL");
	}
}
