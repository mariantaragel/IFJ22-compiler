#include "precedence_parser.h"
#include "precedence_table.h"
#include "precedence_rules.h"
#include "precedence_stack.h"
#include "token_array.h"
#include "token.h"

pstack_symbol_t* create_dollar_pstack_symbol(){
	// allocate precedence stack symbol
	pstack_symbol_t* pstack_symbol = malloc(sizeof(*pstack_symbol));
	if(pstack_symbol == NULL) return NULL;

	// initialize new symbol to be DOLLAR symbol
	pstack_symbol->handle_start = false;
	pstack_symbol->prec_rule_element = (prec_rule_elem_t){TERM, -1};
	pstack_symbol->prec_table_index = DOLLAR_INDEX;
	pstack_symbol->token_array = NULL;

	// return initialized precedence stack symbol
	return pstack_symbol;
}

pstack_symbol_t* create_terminal_pstack_symbol(token_t* token){
	// allocate precedence stack symbol
	pstack_symbol_t* pstack_symbol = malloc(sizeof(*pstack_symbol));
	if(pstack_symbol == NULL){
		return NULL;
	} 

	// create new token array
	token_array_t* token_array = token_array_create();
	if(token_array == NULL){
		free(pstack_symbol);
		return NULL;
	}

	// add token to token array
	if(token_array_add_token(token_array, token) != 0){
		free(pstack_symbol);
		token_array_free(token_array);
		return NULL;
	}

	// initialize new symbol to be TERMINAL symbol
	pstack_symbol->handle_start = false;
	pstack_symbol->prec_rule_element = (prec_rule_elem_t){TERM, token->type};
	pstack_symbol->prec_table_index = DOLLAR_INDEX; // TODO assign correct prec_table_index
	pstack_symbol->token_array = token_array;

	// return initialized precedence stack symbol
	return pstack_symbol;
}

/*
pstack_symbol_t* create_nonterminal_pstack_symbol(pstack_symbol_t* symbol1, pstack_symbol_t* symbol2, pstack_symbol_t* symbol3){

}*/