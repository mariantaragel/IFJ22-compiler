#ifndef PRECEDENCE_PARSER_H
#define PRECEDENCE_PARSER_H

#include "precedence_table.h"
#include "precedence_rules.h"
#include "precedence_stack.h"
#include "token_array.h"
#include "token.h"

/**
 * @brief 
 * 
 * @return pstack_symbol_t* 
 */
pstack_symbol_t* create_dollar_pstack_symbol();

/**
 * @brief
 * 
 * @param token 
 * @return pstack_symbol_t*
 */
pstack_symbol_t* create_terminal_pstack_symbol(token_t* token);

/**
 * @brief
 * 
 * @param symbol
 * @return pstack_symbol_t*
 */
void free_pstack_symbol(pstack_symbol_t* symbol);

/**
 * @brief Converts token to index of the precedence table.
 * 
 * @param ptoken Token to convert.
 * @return prec_table_index_t Returns index of the token in precedence table.
 */
prec_table_index_t get_index_from_token(token_t* ptoken);

/**
 * @brief Finds the value of index of token and top in the precedence table. 
 * 
 * @param x Enum value of sent index.
 * @return int The index number in precedence table.
 */
int position_of(x);

/**
 * @brief Works out reduce operation on the precedence stack.
 * 
 * @param stack Precedence stack.
 * @return int
 */
int reduce(pstack_t* stack);

/**
 * @brief Parses expression bottom-up. Complements top_down parser.
 * 
 * @param token_array_t* Token array created by top-down parser.
 * @return token_array_t* Returns parsed array to top-down.
 */
token_array_t* parse_expression(/*token_array_t*/);


#endif
