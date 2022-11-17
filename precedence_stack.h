#ifndef PRECEDENCE_STACK_H
#define PRECEDENCE_STACK_H

#include <stdbool.h> // bool
#include <stdlib.h>  // size_t

#include "precedence_table.h"
#include "precedence_rules.h"
#include "token_array.h"
#include "token.h"


/**
 * @brief Type of symbol stored in precedence stack.
 * 
 */
typedef struct{
    prec_rule_elem_t prec_rule_element;
    token_array_t* token_array;
}pstack_symbol_t;

/**
 * @brief Precedence stack structure type.
 * 
 */
typedef struct pstack pstack_t;

/**
 * @brief Creates precedence stack and returns pointer to it.
 * 
 * @return pstack_t* Pointer to created precedence stack on sucess. Otherwise NULL is returned.
 */
pstack_t * pstack_create();

/**
 * @brief Frees precedence stack. Including all symbols and symbol token arrays stored in it.
 * 
 * @param pstack Precedence stack to be freed.
 */
void pstack_free(pstack_t *pstack);

prec_table_index_t pstack_get_top_terminal_prec_table_index(pstack_t* pstack);

void pstack_set_top_terminal_handle_start(pstack_t* pstack, bool handle_start);

bool pstack_is_top_handle_start(pstack_t* pstack);


int pstack_push_terminal(pstack_t* pstack, token_t* token);

int pstack_push_nonterminal(pstack_t* pstack, token_array_t* token_array);



token_array_t* pstack_pop(pstack_t* pstack, bool* is_terminal);

/**
 * @brief Returns number of symbols stored in precedence stack.
 * 
 * @param pstack Precedence stack.
 * @return size_t Number of symbols in precedence stack.
 */
size_t pstack_get_size(pstack_t* pstack);


#endif