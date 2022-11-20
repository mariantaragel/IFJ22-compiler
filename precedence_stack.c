#include <stdbool.h> // bool
#include <stdlib.h>  // NULL, size_t

#include "precedence_stack.h"
#include "precedence_table.h"
#include "precedence_rules.h"

/**
 * @brief Precedence stack element.
 * 
 */
typedef struct pstack_elem{
	token_array_t *token_array;

	bool handle_start;
	bool is_terminal;
	prec_table_index_t prec_table_index;

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

	pstack->size = 1;

	// add initial dollar symbol
	pstack->top_ptr = malloc(sizeof(*pstack->top_ptr));
	if(pstack->top_ptr == NULL){
		free(pstack);
		return NULL;
	}
	pstack->top_ptr->is_terminal = true;
	pstack->top_ptr->handle_start = false;
	pstack->top_ptr->prec_table_index = DOLLAR_INDEX;
	pstack->top_ptr->token_array = NULL;

	pstack->top_ptr->next_ptr = NULL;

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

		// free token array of element
		token_array_free(tmp_elem_ptr->token_array);

		// free old top element
		free(tmp_elem_ptr);
	}

	// free precedence stack structure itself
	free(pstack);
}

prec_table_index_t pstack_get_top_terminal_prec_table_index(pstack_t* pstack){
	pstack_elem_t* cur_elem_ptr = pstack->top_ptr;

	// iterate trough elements of pstack until either end is reached or terminal is found
	while(cur_elem_ptr != NULL && cur_elem_ptr->is_terminal != true){
		cur_elem_ptr = cur_elem_ptr->next_ptr;
	}

	if(cur_elem_ptr == NULL){
		return ERROR_INDEX;
	}
	else{
		return cur_elem_ptr->prec_table_index;
	}
}

void pstack_set_top_terminal_handle_start(pstack_t* pstack, bool handle_start){
	pstack_elem_t* cur_elem_ptr = pstack->top_ptr;

	// iterate trough elements of pstack until either end is reached or terminal is found
	while(cur_elem_ptr != NULL && cur_elem_ptr->is_terminal != true){
		cur_elem_ptr = cur_elem_ptr->next_ptr;
	}

	if(cur_elem_ptr != NULL){
		cur_elem_ptr->handle_start = handle_start;
	}
}

bool pstack_is_top_handle_start(pstack_t* pstack){
	if(pstack == NULL || pstack->size == 0){
		return false;
	}
	else{
		return pstack->top_ptr->handle_start;
	}
}


int pstack_push_terminal(pstack_t* pstack, token_t* token){
	if(token == NULL) return -1;

	// create new pstack element
	pstack_elem_t* new_elem_ptr = malloc(sizeof(*new_elem_ptr));
	if(new_elem_ptr == NULL){
		return -1;
	}

	// initialize new pstack element
	new_elem_ptr->token_array = token_array_create();
	if(new_elem_ptr->token_array == NULL){
		free(new_elem_ptr);
		return -1;
	}

	if(token_array_push_token(new_elem_ptr->token_array, token) != 0){
		token_array_free(new_elem_ptr->token_array);
		free(new_elem_ptr);
		return -1;
	}

	new_elem_ptr->handle_start = false;
	new_elem_ptr->is_terminal = true;
	new_elem_ptr->prec_table_index = get_prec_table_index_of_token(token);

	// make new element to element of pstack
	new_elem_ptr->next_ptr = pstack->top_ptr;
	pstack->top_ptr = new_elem_ptr;

	// update pstack size
	++(pstack->size);

	// return success
	return 0;
}

int pstack_push_nonterminal(pstack_t* pstack, token_array_t* token_array){
	if(token_array == NULL) return -1;

	// create new pstack element
	pstack_elem_t* new_elem_ptr = malloc(sizeof(*new_elem_ptr));
	if(new_elem_ptr == NULL){
		return -1;
	}

	// initialize new pstack element
	new_elem_ptr->token_array = token_array;

	new_elem_ptr->handle_start = false;
	new_elem_ptr->is_terminal = false;
	new_elem_ptr->prec_table_index = ERROR_INDEX;

	// make new element to element of pstack
	new_elem_ptr->next_ptr = pstack->top_ptr;
	pstack->top_ptr = new_elem_ptr;

	// update pstack size
	++(pstack->size);

	// return success
	return 0;
}


token_array_t* pstack_pop(pstack_t* pstack, prec_rule_elem_t* prec_rule_elem){
	if(pstack == NULL || pstack->top_ptr == NULL){
		if(prec_rule_elem != NULL){
			*prec_rule_elem = PREC_RULE_ELEM_NAN;
		}
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
		token_array_t* token_array = tmp_elem_ptr->token_array;

		if(prec_rule_elem != NULL){
			if(tmp_elem_ptr->is_terminal){
				// expression token array has only 1 token which represents terminal symbol
				*prec_rule_elem = PREC_RULE_ELEM_TERM(token_array->array[0]->type);
			}
			else{
				// expression token array has only X tokens representing nonterminal symbol
				*prec_rule_elem = PREC_RULE_ELEM_EXPR;
			}
		}
		
		free(tmp_elem_ptr);

		return token_array;
	}
}

size_t pstack_get_size(pstack_t* pstack){
	return pstack->size;
}
