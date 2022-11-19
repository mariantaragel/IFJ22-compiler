/****
 ** expgen.c
 ** Řešení IFJ-PROJEKT, 13.11.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file expgen.c
 * @author xhorva17
 * @brief Expression generation implementation.
 * @date 2022-13-11
 */

#include <stdio.h>
#include "expgen.h"
#include "token_array.h"
#include "generator.h"
// #include "generator.c"
#include "scanner.h"
#include "error.h"

// int main() {
//     token_t * token; // Scan tokens in postfix...
//     token_array_t * tarr = token_array_create();
//     if(tarr == NULL) {
//         return 1;
//     }


// 	do {
// 		token = get_token();
// 		t_print(token);
// 		token_array_push_token(tarr, token);
// 		if(error == INTERNAL_ERROR) {
// 			fprintf(stderr, "INTERNAL_ERROR\n");
// 			return error;
// 		} else if (error == LEXICAL_ERROR) {
// 			fprintf(stderr, "LEXICAL_ERROR\n");
// 			return error;
// 		}
// 	} while(token->type != END && token->type != EPILOG);
//     // token_array_reverse(tarr);

//     token_array_expr_print(tarr, stdout);


// }


// void gen_expression(AST_node_t * node, generator_context_t * g_context) {
//     // Input node with postfix expression...
// }


// void getit(token_array_t * tarr) {
    
// }


/*
    Push literal or variable name onto the stack.
        -> PUSHS float@0x1.2p+1
        -> PUSHS int@123
        -> PUSHS string@ahoj\032svete
        -> PUSHS LF@$var1

    If operator found, create result and store on top of stack.
        -> stack s...
*/





