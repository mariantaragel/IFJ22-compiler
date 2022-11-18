#include "precedence_parser.h"
#include "precedence_table.h"
#include "precedence_rules.h"
#include "precedence_stack.h"
#include "token_array.h"
#include "token.h"
#include "error.h"


prec_rule_elem_t get_prec_rule_elem_of_token_array(token_array_t* token_array, bool is_term){
	if(token_array == NULL || token_array->token_count == 0){
		return PREC_RULE_ELEM_NAN;
	}
	else if(is_term == true){
		return PREC_RULE_ELEM_TERM(token_array->array[0]->type);
	}
	else{
		return PREC_RULE_ELEM_EXPR;
	}
}

error_codes_t reduce(pstack_t *pstack){
	token_array_t *ta1, *ta2, *ta3;
	ta1 = ta2 = ta3 = NULL;
	bool is_term1, is_term2, is_term3;
	is_term1 = is_term2 = is_term3 = false;

	bool too_many_prec_rule_elements = false;
	if(pstack_is_top_handle_start(pstack) == false){
		ta3 = pstack_pop(pstack, &is_term3);
		if(pstack_is_top_handle_start(pstack) == false){
			ta2 = pstack_pop(pstack, &is_term2);
			if(pstack_is_top_handle_start(pstack) == false){
				ta1 = pstack_pop(pstack, &is_term1);
				if(pstack_is_top_handle_start(pstack) == false){
					too_many_prec_rule_elements = true;
				}
			}
			else{
				ta1 = ta2; ta2 = ta3; ta3 = NULL;
				is_term1 = is_term2; is_term2 = is_term3; is_term3 = false;

			}
		}
		else{
			ta1 = ta3; ta3 = NULL;
			is_term1 = is_term3; is_term3 = false;
		}
	}

	if(too_many_prec_rule_elements == true){
		token_array_free(ta1);
		token_array_free(ta2);
		token_array_free(ta3);
		return SYNTAX_ERROR;
	}

	pstack_set_top_terminal_handle_start(pstack, false);

	prec_rule_elem_t e1, e2, e3;
	e1 = get_prec_rule_elem_of_token_array(ta1, is_term1);
	e2 = get_prec_rule_elem_of_token_array(ta2, is_term2);
	e3 = get_prec_rule_elem_of_token_array(ta3, is_term3);

	prec_rule_action_t rule_action = match_precedence_rule(e1, e2, e3);

	error_codes_t res = OK;

	switch(rule_action){
		case CONVERT_TO_EXPR:
			if(pstack_push_nonterminal(pstack, ta1) == 0)
				ta1 = NULL;
			else
				res = INTERNAL_ERROR;
			break;

		case REMOVE_BRACKETS:
			if(pstack_push_nonterminal(pstack, ta2) == 0)
				ta2 = NULL;
			else
				res = INTERNAL_ERROR;
			break;

		case POSTFIX_MERGE:
			if(	token_array_append_tokens(ta3, ta1) == 0 &&
				token_array_append_tokens(ta3, ta2) == 0 &&
				pstack_push_nonterminal(pstack, ta3) == 0){
					ta3 = NULL;
			}
			else{
				res = INTERNAL_ERROR;
			}
			break;
		default:
			res = SYNTAX_ERROR;
	}

	token_array_free(ta1);
	token_array_free(ta2);
	token_array_free(ta3);

	return res;
}

token_array_t* parse_expression(token_array_t* expr_token_array){
	if(expr_token_array == NULL || expr_token_array->token_count == 0)
		return expr_token_array;

	token_array_reverse(expr_token_array);

	pstack_t *pstack = pstack_create();
	if(pstack == NULL){
		error = INTERNAL_ERROR;
		token_array_free(expr_token_array);
		return NULL;
	}

	error_codes_t res;

	prec_table_index_t top_term_prec_index;

	token_t* input_token;
	prec_table_index_t input_term_prec_index;

	prec_parser_operation_t prec_parser_operation;

	bool done = false;

	input_token = token_array_pop_token(expr_token_array);
	input_term_prec_index = get_prec_table_index_of_token(input_token);

	do{
		top_term_prec_index = pstack_get_top_terminal_prec_table_index(pstack);
		
		prec_parser_operation = precedence_table[top_term_prec_index][input_term_prec_index];
		
		switch(prec_parser_operation){
			case S:
				pstack_set_top_terminal_handle_start(pstack, true);
			case E:
				if(pstack_push_terminal(pstack, input_token) != 0){
					error = INTERNAL_ERROR;
					done = true;
				}
				input_token = token_array_pop_token(expr_token_array);
				break;
			case R:
				if((res = reduce(pstack)) != OK){
					error = res;
					done = true;
				}
				break;
			default:
				error = SYNTAX_ERROR;
				done = true;
		}

		if(pstack_get_size(pstack) == 2){
			if(pstack_get_top_terminal_prec_table_index(pstack) == DOLLAR_INDEX){
				if(input_token == NULL){
					done = true;
				}
			}
		}
		
	}while(!done);

	return expr_token_array;
}
