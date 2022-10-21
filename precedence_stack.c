#include <stdbool.h> // bool
#include <stdlib.h>  // NULL, size_t

#include "precedence_stack.h"


/**
 * @brief Precedence stack element.
 * 
 */
typedef struct pstack_elem{
    pstack_symbol_t* symbol;

    struct pstack_elem * next_ptr;
}pstack_elem_t;

/**
 * @brief Precedence stack structure.
 * 
 */
struct pstack{
	size_t size;
    pstack_elem_t * top_ptr;
};



pstack_t * pstack_create(){
	// allocate new precedence stack structure
    pstack_t *pstack = malloc(sizeof(*pstack));
    if(pstack == NULL) return NULL;

	// initialize empty pstack
	pstack->size = 0;
	pstack->top_ptr = NULL;

	// return allocated precedence stack structure with DOLLAR element
    return pstack;
}

void pstack_free(pstack_t* pstack){
	pstack_elem_t* tmp_elem_ptr;

	// while all elements have not been freed 
	while(pstack->top_ptr != NULL){

		// next element of current top element becomes new top element
		tmp_elem_ptr = pstack->top_ptr;
		pstack->top_ptr = tmp_elem_ptr->next_ptr;

		// free token array of symbol
		if(tmp_elem_ptr->symbol->token_array != NULL){
			token_array_free(tmp_elem_ptr->symbol->token_array);
		}

		// free symbol
		free(tmp_elem_ptr->symbol);

		// free old top element
		free(tmp_elem_ptr);
	}

	// free precedence stack structure itself
	free(pstack);
}

pstack_symbol_t* pstack_top_terminal(pstack_t* pstack){
	pstack_elem_t* cur_elem_ptr = pstack->top_ptr;

	// iterate trough elements of pstack until either end is reached or terminal is found
	while(cur_elem_ptr != NULL && is_prec_rule_elem_term(cur_elem_ptr->symbol->prec_rule_element) != true){
		cur_elem_ptr = cur_elem_ptr->next_ptr;
	}

	if(cur_elem_ptr == NULL){
		return NULL;
	}
	else{
		return cur_elem_ptr->symbol;
	}
}

pstack_symbol_t* pstack_top(pstack_t* pstack){
	if(pstack->top_ptr != NULL){
		return pstack->top_ptr->symbol;
	}
	else{
		return NULL;
	}
}

int pstack_push(pstack_t* pstack, pstack_symbol_t* pstack_symbol){
	// create new pstack element
	pstack_elem_t* new_elem_ptr = malloc(sizeof(*new_elem_ptr));
	if(new_elem_ptr == NULL){
		return -1;
	}

	// initialize new pstack element
	new_elem_ptr->symbol = pstack_symbol;

	// make new element to element of pstack
	new_elem_ptr->next_ptr = pstack->top_ptr;
	pstack->top_ptr = new_elem_ptr;

	// update pstack size
	++(pstack->size);

	// return success
	return 0;
}

pstack_symbol_t* pstack_pop(pstack_t* pstack){
	if(pstack->top_ptr == NULL){
		return NULL;
	}
	else{
		// update pstack size
		--(pstack->size);

		// store top element of pstack
		pstack_elem_t* tmp_elem_ptr = pstack->top_ptr;

		// remove top element from pstack
		pstack->top_ptr = pstack->top_ptr->next_ptr;

		// free old top element and return symbol stored in it
		pstack_symbol_t* top_symbol_ptr = tmp_elem_ptr->symbol;
		
		free(tmp_elem_ptr);

		return top_symbol_ptr;
	}
}

size_t pstack_get_size(pstack_t* pstack){
	return pstack->size;
}
