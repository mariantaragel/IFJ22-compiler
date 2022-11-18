#ifndef TOKEN_ARRAY_H
#define TOKEN_ARRAY_H

#include "token.h"
#include <stdio.h>

/**
 * @brief Token array type.
 * 
 */
typedef struct{
    size_t token_count; // Number of tokens stored in token array.
    size_t size;        // Token array size.

    token_t ** array;   // Token array.
}token_array_t;

/**
 * @brief Constructor: Allocates and initialises token array structure and returns pointer to it.
 * 
 * @return token_array_t* Pointer to created token array structure on success, otherwise NULL is returned. 
 */
token_array_t* token_array_create();

/**
 * @brief Destructor: Frees all tokens in token array structure and token array structure itself.
 * 
 * @param token_array Token array structure to be freed.
 */
void token_array_free(token_array_t* token_array);

/**
 * @brief Pushes new token to token array.
 * 
 * @param token_array Token array.
 * @param token New token to be added.
 * @return int Returns 0 on success, otherwise non zero value is returned.
 */
int token_array_push_token(token_array_t* token_array, token_t *token);

/**
 * @brief Pops token from token array.
 * 
 * @param token_array Token array.
 * @return token_t* Returns popped token from token array on success, otherwise, if token array is empty, NULL is returned.
 */
token_t* token_array_pop_token(token_array_t* token_array);

/**
 * @brief Appends all tokens from token_array2 to token_array1. 
 * On success, all tokens from token_array2 are appended to token_array1, token_array2 becomes empty (token_count == 0).
 * On failure, both token arrays remain unchanged.
 * 
 * @param token_array1
 * @param token_array2
 * @return int Returns 0 on success, otherwise non zero value is returned.
 */
int token_array_append_tokens(token_array_t* token_array1, token_array_t* token_array2);

/**
 * @brief Prints token array as expression to specified stream (e.g. stdout).
 * 
 * @param expr_token_array Token array to be printed.
 * @param fp Stream to be printed in.
 */
void token_array_expr_print(token_array_t* expr_token_array, FILE* fp);

/**
 * @brief Reverse order of tokens in token array.
 * 
 * @param token_array Token array to be reversed.
 */
void token_array_reverse(token_array_t* token_array);

#endif