#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "precedence_table.h"
#include "precedence_stack.h"
#include "token.h" 
#include "scanner.h"
#include "error.h"

//errors, doxygen

static prec_table_index_t get_index_from_token(token_t* ptoken){

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

static int rule_reduce(char rule_arr[], char expression[]) {
	
}

static int rule_end()(char rule_arr[], char expression[]) {
	
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

parse_expression(//tokeny z top-down (asi pointer na ulozena data){
	
	pstack_t *stack;
	pstack_create(&stack);											//inicializace zasobniku
	//create_dollar_pstack_symbol(stack);
	
	char expression[20];
	expression[0] = 36;
	int rule_counter = 0;
	char rule_arr[3];
	
	bool done = false;
	do {
		token_t *ptoken = get_token();								//get_token
		prec_table_index_t index = get_index_from_token(&ptoken);	//znak na vstupu -> b
		pstack_symbol_t a = pstack_top(&stack); 					//funkce top -> a
		
		switch (prec_table[position_of(a)][position_of(index)]) {
			//equal
			case E:													//push b & precti dalsi
				pstack_push(&stack, index);
			//shift	
			case S:													//a -> a< & push b & precti dalsi
				pstack_push(&stack, index);
				a = pstack_top(&stack); 
				a->handle_start = true;
				update_expression(expression);
			//reduce
			case R:													// if top "<y" && r:A->y (rule) -> "<y" -> E & r na výstup
				is_rule_applicable(a);								// if top true
				rule_reduce(rule_arr);	
			//err/end
			case X:													//ERR || (if b = $ && a=$E -> expression_valid END) 
				if (index == DOLLAR_INDEX && a == DOLLAR) {
					rule_end(expression);
					done = true;
				}
				else {
					//SYNTAX_ERR;
				}
			default:
					//ERR
		}
		
	} while (!done);		
	
	pstack_free(&stack);
	return ;
}


	



