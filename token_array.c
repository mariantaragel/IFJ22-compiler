#include <stdlib.h> // NULL
#include <stdint.h> // SIZE_MAX

#include "token_array.h"
#include "token.h"

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

int token_array_reallocate(token_array_t* token_array, size_t new_size){
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

int token_array_add_token(token_array_t* token_array, token_t *token){
    if(token_array != NULL){

        // if token array is full, double its size
        if(token_array->token_count == token_array->size){

            // check for overflow, when doubling array size
            if(token_array->size > SIZE_MAX/2){
                return -1;
            }

            // new token array size
            size_t new_size = token_array->size * 2;

            // reallocate token array
            if(token_array_reallocate(token_array, new_size) == -1){
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
    else{
        // return failure
        return -1;
    }
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
    if(token_array_reallocate(token_array1, new_size) == -1){
        return -1;
    }

    // move all tokens from token_array2 to token_array1
    for(size_t i = 0; i < token_array2->token_count; ++i){
        // add token from token_array2 to token_array1 and update token count
        token_array1->array[token_array1->token_count] = token_array2->array[i];
        (token_array1->token_count)++;
    }

    // set token count of second array to zero and then free it
    token_array2->token_count = 0;
    token_array_free(token_array2);

	// return success
	return 0;
}