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
	pstack_symbol->prec_rule_element = PREC_RULE_ELEM_TERM(-1);
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
	pstack_symbol->prec_rule_element = PREC_RULE_ELEM_TERM(token->type);
	pstack_symbol->prec_table_index = DOLLAR_INDEX; // TODO assign correct prec_table_index
	pstack_symbol->token_array = token_array;

	// return initialized precedence stack symbol
	return pstack_symbol;
}

/*
pstack_symbol_t* create_nonterminal_pstack_symbol(pstack_symbol_t* symbol1, pstack_symbol_t* symbol2, pstack_symbol_t* symbol3){

}*/

//errors, doxygen

prec_table_index_t get_index_from_token(token_t* ptoken){

	switch (ptoken->token_type_t) {
		case ADD:
		case SUB:
		case CONCAT:
			return ADD_SUB_CONC_INDEX;
		case MUL:
		case DIV:
			return MUL_DIV_INDEX;
		case LT:
		case GT:
		case LTE:
		case GTE:
			return REL_INDEX;
		case EQ:
		case NEQ:
			return IDENTITY_INDEX;
		case LB:
			return LEFT_BR_INDEX;
		case RB:
			return RIGHT_BR_INDEX;
		case STR_T:
		case INT_T:
		case FLT_T:
		case NULL_T:
		case VOID_T:
		case NULL_T:
		case NFLT_T:
		case NINT_T:
		case NSTR_T:
			return LIT_INDEX;
		default:
			return DOLLAR_INDEX;
	}
}

int position_of(x) {
	
	switch(x){
		case ADD_SUB_CONC_INDEX:
			return 0;
		case MUL_DIV_INDEX:
			return 1;
		case IDENTITY_INDEX:
			return 2;
		case REL_INDEX:
			return 3;
		case LEFT_BR_INDEX:
			return 4;
		case RIGHT_BR_INDEX:
			return 5;
		case LIT_INDEX:	
			return 6;
		case DOLLAR_INDEX:
			return 7;
		default:
			return 7;
	}	
}

/*
static char prec_char_get(prec_table_index_t index) {
		
		switch(index){
			case LIT_INDEX:
				return 105;	
		} 

}
*/

int reduce(pstack_t* stack) {
	pstack_symbol_t* symbols[3];
	symbols[0] = NULL;
	symbols[1] = NULL;
	symbols[2] = NULL;
	
	symbols[0] = pstack_pop(stack);
	if (symbols[0] == NULL)
	//symbiols [0] -> handle true
	//if true -> handle = false
	//3x
	//if top != handle true -> err
	//match precedence rule
	//apply rule
	
	//ok
	//internmal_err
	//syntax_err
}

int rule_end()() {
	
}

int is_rule_applicable(pstack_symbol_t a){							//if handle found ("<y")
	if (a->handle_start == true) {
		return 1;
	}
	else {
		return 0;
	}
}


//void update_expression(char expression)

//void 

token_array_t* parse_expression(//token_array_type){
	
	pstack_t *stack;
	pstack_create(&stack);											//inicializace zasobniku
	//create_dollar_pstack_symbol(stack);
	
	char expression[20];
	expression[0] = 36;
	int rule_counter = 0;
	char rule_arr[3];
	bool internal_err = false;
	bool syntax_err = false;	
	bool done = false;
	do {
		token_t *ptoken = get_token();								//"get_token" - pozdeji z token array (top-down)
		prec_table_index_t index = get_index_from_token(ptoken);	//znak na vstupu -> b
		pstack_symbol_t* top_terminal = pstack_top_terminal(stack); //funkce top -> a
		if (top_terminal == NULL) {
			internal_err = true;
			break;
		}
		
		switch (prec_table[top_terminal->prec_table_index][index]) {
			//equal
			case E:
				pstack_symbol_t* new_terminal_symbol = create_terminal_pstack_symbol(ptoken);			//push b & precti dalsi
				if(new_terminal_symbol == NULL) {
					internal_err = true;
					done = true;				
				}
				else {
					if (pstack_push(&stack, new_terminal_symbol) != 0) {
						free_pstack_symbol(new_terminal_symbol);
						internal_err = true;
						done = true;
					}
				}
				//zde nacist
				break;
			//shift	
			case S:													//a -> a< & push b & precti dalsi
				pstack_symbol_t* new_terminal_symbol = create_terminal_pstack_symbol(ptoken);
				if(new_terminal_symbol == NULL) {
					internal_err = true;
					done = true;				
				}
				else {
					if (pstack_push(&stack, new_terminal_symbol) != 0) {
						free_pstack_symbol(new_terminal_symbol);
						internal_err = true;
						done = true;
					}
					else{
						top_terminal->handle_start = true;
					}
				}
				//zde nacist
				break;
			//reduce
			case R:													// if top == "<y" && r:A->y (rule) -> "<y" -> E & r na výstup
				reduce(stack);
				
				break;	
			//err/end
			case X:													//ERR || (if b = $ && a=$E -> expression_valid END) 
				if (index == DOLLAR_INDEX && pstack_get_size(stack) == 2 && pstack_top(stack)->prec_rule_element == EXPR) {
					syntax_err = false;
					done = true;
				}
				else {
					syntax_err = true;
					done = true;
				}
		}
		
	} while (!done);		
	
	pstack_free(&stack);
	return ;
}
