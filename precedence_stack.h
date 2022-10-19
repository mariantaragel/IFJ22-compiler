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
    bool handle_start;

    prec_rule_elem_t prec_rule_element;
    prec_table_index_t prec_table_index;

    token_array_t* token_array;
}pstack_symbol_t;

/**
 * @brief Precedence stack structure type.
 * 
 */
typedef struct pstack pstack_t;



/**
 * @brief Creates precedence stack, adds first DOLLAR element and returns pointer to it.
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

/**
 * @brief Returns topmost terminal symbol from precedence stack.
 * 
 * @param pstack Precedence stack.
 * @return pstack_symbol_t* Returns topmost terminal symbol on success, otherwise NULL is returned.
 */
pstack_symbol_t* pstack_top_terminal(pstack_t* pstack);

/**
 * @brief Returns top symbol from precedence stack.
 * 
 * @param pstack Precedence stack.
 * @return pstack_symbol_t* Returns top symbol on success, otherwise NULL is returned.
 */
pstack_symbol_t* pstack_top(pstack_t* pstack);


/**
 * @brief Adds new symbol to top of precedence stack.
 * 
 * @param pstack Precedence stack.
 * @param pstack_symbol Symbol to be pushed to precedence stack.
 * @return int 
 */
int pstack_push(pstack_t* pstack, pstack_symbol_t* pstack_symbol);

/**
 * @brief Removes and returns top symbol from precedence stack.
 * 
 * @param pstack Precedence stack.
 * @return pstack_symbol_t* Returns former top symbol of precedence stack on sucess, otherwise NULL is returned.
 */
pstack_symbol_t* pstack_pop(pstack_t* pstack);

/**
 * @brief Returns number of symbols stored in precedence stack.
 * 
 * @param pstack Precedence stack.
 * @return size_t Number of symbols in precedence stack.
 */
size_t pstack_get_size(pstack_t* pstack);


#endif