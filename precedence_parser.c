/**
 * @file percedence_parser.c
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Implementation of precedence parser.
 * @date 2022-11-26
 * 
 */

#include "precedence_parser.h"
#include "precedence_table.h"
#include "precedence_rules.h"
#include "precedence_stack.h"
#include "token_array.h"
#include "token.h"
#include "error.h"

error_codes_t reduce(pstack_t *pstack){
	// token arrays of precedence reduction rule elements of handle at the top of precedence stack
	token_array_t *ta1, *ta2, *ta3;
	ta1 = ta2 = ta3 = NULL;

	// precedence rule elements representing handle at the top of precedence stack
	prec_rule_elem_t e1, e2, e3;
	e1 = e2 = e3 = PREC_RULE_ELEM_NAN;

	// get handle from precedence stack (handle is popped from stack in reversed)
	bool too_many_prec_rule_elements = false;
	if(pstack_is_top_handle_start(pstack) == false){
		ta3 = pstack_pop(pstack, &e3);
		if(pstack_is_top_handle_start(pstack) == false){
			ta2 = pstack_pop(pstack, &e2);
			if(pstack_is_top_handle_start(pstack) == false){
				ta1 = pstack_pop(pstack, &e1);
				if(pstack_is_top_handle_start(pstack) == false){
					// start of handle was not found after popping 3 elements from precedence stack (maximum rule elements == 3)
					too_many_prec_rule_elements = true;
				}
			}
			else{
				// shift precedence rule elements to start at e1
				//   e1   e2   e3        e1   e2   e3 
				// |NULL| e2 | e3 | -> | e2 | e3 |NULL|
				e1 = e2; e2 = e3; e3 = PREC_RULE_ELEM_NAN;

				// shift token arrays to start at ta1
				ta1 = ta2; ta2 = ta3; ta3 = NULL;
			}
		}
		else{
			// shift precedence rule elements to start at e1
			//   e1   e2   e3        e1   e2   e3
			// |NULL|NULL| e3 | -> | e3 |NULL|NULL|
			e1 = e3; e3 = PREC_RULE_ELEM_NAN;

			// shift token arrays to start at ta1
			ta1 = ta3; ta3 = NULL;
		}
	}

	// if start of handle was not found, there were too many rule elements (maximum == 3), than expression has invalid syntax
	if(too_many_prec_rule_elements == true){
		token_array_free(ta1);
		token_array_free(ta2);
		token_array_free(ta3);
		return SYNTAX_ERROR;
	}

	// remove handle start of top terminal
	pstack_set_top_terminal_handle_start(pstack, false);

	error_codes_t res = OK;

	// get precedence rule action based on precedence rule elements
	prec_rule_action_t rule_action = match_precedence_rule(e1, e2, e3);

	// apply precedence rule action
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

	// free temporary token arrays
	token_array_free(ta1);
	token_array_free(ta2);
	token_array_free(ta3);
	
	// return result (OK, SYNTAX_ERROR or INTERNAL_ERROR)
	return res;
}

token_array_t* parse_expression(token_array_t* expr_token_array){
	error = OK;

	// if expression token array is empty, syntax is OK and return empty expression array back
	if(expr_token_array == NULL || expr_token_array->token_count == 0)
		return expr_token_array;

	// reverse tokens of expression token array, so tokens will be popped in correct order from token array 
	token_array_reverse(expr_token_array);

	// create precedence stack
	pstack_t *pstack = pstack_create();
	if(pstack == NULL){
		error = INTERNAL_ERROR;
		token_array_free(expr_token_array);
		return NULL;
	}

	error_codes_t prec_parser_error = OK;

	token_t* input_token;
	 
	// indexes of precedence table
	prec_table_index_t top_term_prec_index;
	prec_table_index_t input_term_prec_index;

	// operation from precedence table
	prec_parser_operation_t prec_parser_operation;

	bool done = false;

	// get new input token
	input_token = token_array_pop_token(expr_token_array);
	
	do{
		// get precedence index of input token
		input_term_prec_index = get_prec_table_index_of_token(input_token);
		
		// get topmost precedence stack terminal index
		top_term_prec_index = pstack_get_top_terminal_prec_table_index(pstack);
		
		// get precedence parser operation from precedence table
		prec_parser_operation = precedence_table_get_operation(top_term_prec_index, input_term_prec_index);

		// execute precedence parser operation
		switch(prec_parser_operation){
			case E:
				// push input token to precedence stack
				if(pstack_push_terminal(pstack, input_token) != 0){
					prec_parser_error = INTERNAL_ERROR;
					done = true;
				}

				// get next input token
				input_token = token_array_pop_token(expr_token_array);
				break;

			case S:
				// set handle start to true of tomost terminal
				pstack_set_top_terminal_handle_start(pstack, true);

				// push input token to precedence stack
				if(pstack_push_terminal(pstack, input_token) != 0){
					prec_parser_error = INTERNAL_ERROR;
					done = true;
				}

				// get next input token
				input_token = token_array_pop_token(expr_token_array);
				break;

			case R:
				// apply reduction rules
				if((prec_parser_error = reduce(pstack)) != OK){
					done = true;
				}
				break;

			default:
				// syntax error occured
				prec_parser_error = SYNTAX_ERROR;
				done = true;
		}

		if(!done){
			// check if precedence stack is in accept state
			if(pstack_get_size(pstack) == 2){
				if(pstack_get_top_terminal_prec_table_index(pstack) == DOLLAR_INDEX){
					if(input_token == NULL){
						// we are at the end of input and precedence stack contains only dollar symbol and nonterminal
						done = true;
					}
				}
			}
		}
	}while(!done);

	// free last input token
	if(input_token != NULL) t_dstr(input_token);

	// free original token array
	token_array_free(expr_token_array);

	if(prec_parser_error != OK){
		// set global error variable, free precedence stack and return NULL
		error = prec_parser_error;
		pstack_free(pstack);
		return NULL;
	}
	else{
		// pop postfix reprezentation of expression token array from precedence stack
		token_array_t* posfix_expr_token_array = pstack_pop(pstack, NULL);
		if(posfix_expr_token_array == NULL){
			error = INTERNAL_ERROR;
		}
		// free precedence stack
		pstack_free(pstack);

		// return posftix expression
		return posfix_expr_token_array;
	}
}
