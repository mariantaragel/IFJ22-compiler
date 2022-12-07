/**
 * @file precedence_stack.h
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Precedence stack interface.
 * @date 2022-11-26
 * 
 */

#ifndef PRECEDENCE_STACK_H
#define PRECEDENCE_STACK_H

#include <stdbool.h> // bool
#include <stdlib.h>  // size_t

#include "precedence_table.h"
#include "precedence_rules.h"
#include "token_array.h"
#include "token.h"

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

/**
 * @brief Get precedence table index of topmost terminal of precedence stack.
 * 
 * @param pstack Precedence stack.
 * @return prec_table_index_t Returns precedence table index of topmost terminal on success, 
 * othewise if topmost terminal doesnt exist ERROR_INDEX is returned.
 */
prec_table_index_t pstack_get_top_terminal_prec_table_index(pstack_t* pstack);

/**
 * @brief Sets handle start indicator of topmost terminal in precedence stack to specified value.
 * If there are no terminals in precedence stack, nothing happens.
 * Otherwise handle start indicator of topmost terminal is set to handle_start value.
 * 
 * @param pstack Precedence stack.
 * @param handle_start Speicifies value to which topmost terminal handle start indicator will be set to.
 */
void pstack_set_top_terminal_handle_start(pstack_t* pstack, bool handle_start);

/**
 * @brief Checks whether top element of precedence stack has handle start set to true.
 * 
 * @param pstack Precedence stack.
 * @return Returns true if top element of precedence stack has handle start indicator set to true, otherwise false is returned.
 */
bool pstack_is_top_handle_start(pstack_t* pstack);

/**
 * @brief Pushes token as terminal to precedence stack.
 * Handle start indicator of newly added terminal is set to false.
 * 
 * @param pstack Precedence stack.
 * @param token Token to be pushed in precedence stack.
 * @return int Returns 0 on success, otherwise if allocation error occured, pstack is NULL or token is NULL non zero value is returned.
 */
int pstack_push_terminal(pstack_t* pstack, token_t* token);

/**
 * @brief Pushes token array as nonterminal to precedence stack.
 * 
 * @param pstack Precedence stack to which new nonterminal will be pushed.
 * @param token_array Token array to be pushed in precedence stack.
 * @return int Returns 0 on success, otherwise if allocation error occured non zero value is returned.
 */
int pstack_push_nonterminal(pstack_t* pstack, token_array_t* token_array);

/**
 * @brief Pops top element from precedence stack, returns token array representing postfix subexpression stored in popped element
 * and stores information to precedence rule element structure, representing popped element as precedence rule element.
 * 
 * @param pstack Precedence stack from which top element will be popped.
 * @param prec_rule_elem Precedence rule element structure where information about popped element will be stored.
 * @return token_array_t* Returns token array representing postfix subexpression of popped element on success, otherwise NULL is returned.
 */
token_array_t* pstack_pop(pstack_t* pstack, prec_rule_elem_t* prec_rule_elem);

/**
 * @brief Returns number of elements stored in precedence stack.
 * 
 * @param pstack Precedence stack.
 * @return size_t Number of elements in precedence stack.
 */
size_t pstack_get_size(pstack_t* pstack);

#endif