#include "abstract_syntax_tree.h"
#include "error.h"
#include "labelgen.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct{
    bool is_in_function;
	char* func_end_label;
}generator_context_t;

error_codes_t gen_var_def_check(char* var_name, generator_context_t* gen_context);
error_codes_t gen_expr_vars_def_checks(AST_node_t* expr_n, generator_context_t* gen_context);
error_codes_t gen_expr(AST_node_t* expr_n, generator_context_t* gen_context);
error_codes_t gen_true_false_jump(char* true_label, char* false_label);
error_codes_t gen_while(AST_node_t* while_n, generator_context_t* gen_context);
error_codes_t gen_if(AST_node_t* if_n, generator_context_t* gen_context);
error_codes_t gen_ass_expr(AST_node_t* ass_expr_n, generator_context_t* gen_context);
error_codes_t gen_func_args_def_checks(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_func_args_push(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_func_call(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_ass_func(AST_node_t* ass_func_n, generator_context_t* gen_context);
error_codes_t gen_standalone_func_call(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_standalone_expr(AST_node_t* expr_n, generator_context_t* gen_context);
error_codes_t gen_body(AST_node_t* body_n, generator_context_t* gen_context);
error_codes_t gen_func_params(AST_node_t* params_n);
error_codes_t gen_func_def(AST_node_t* func_def_n, generator_context_t* gen_context);
error_codes_t gen_missing_return(AST_node_t* missing_return_n, generator_context_t* gen_context);
error_codes_t gen_return(AST_node_t* return_n, generator_context_t* gen_context);
error_codes_t gen_func_def_flags(AST_node_t* used_func_list_n);
error_codes_t gen_built_in_functions(AST_node_t* used_func_list_n);
error_codes_t gen_var_defs(AST_node_t* used_vars_list_n, generator_context_t* gen_context);
error_codes_t gen_prog(AST_node_t* prog_n, generator_context_t* gen_context);
generator_context_t* generator_context_create();
void generator_context_free(generator_context_t* gen_context);

void G(const char* fmt, ...){
    va_list arg_list;
    va_start(arg_list, fmt);

    vfprintf(stdout, fmt, arg_list);
	printf("\n");

    va_end(arg_list);
}

// OK
error_codes_t gen_var_def_check(char* var_name, generator_context_t* gen_context){
	// get unique label ?var_initialised
	char* var_init_ok_label = gen_label(NULL, "?var_init_ok", NULL, true);
	if(var_init_ok_label == NULL) return INTERNAL_ERROR;

	// check whether variable with var_name was initialised
	if(gen_context->is_in_function == true){
		G("TYPE GF@_tmp LF@%s", var_name);
	}
	else{
		G("TYPE GF@_tmp GF@%s", var_name);
	}

	G("JUMPIFEQ %s GF@_tmp string@", var_init_ok_label);
	G("EXIT int@5"); // use of undefined variable
	G("LABEL %s", var_init_ok_label);

	free(var_init_ok_label);

	return OK;
}

// OK
error_codes_t gen_expr_vars_def_checks(AST_node_t* expr_n, generator_context_t* gen_context){
	error_codes_t res;

	token_array_t* expr = expr_n->data.expression;
	char* var_name;

	for(size_t i = 0; i < expr->token_count; ++i){
		if(expr->array[i]->type == VAR_ID){
			var_name = expr->array[i]->sval;

			if((res = gen_var_def_check(var_name, gen_context)) != OK) return res;
		}
	}

	return OK;
}

error_codes_t gen_expr(AST_node_t* expr_n, generator_context_t* gen_context){
	// generate code to evaluate expression, including type checks
	// expression result should be at the top of data stack

	return OK;
}

// OK
error_codes_t gen_true_false_jump(char* true_label, char* false_label){
	char* next1 = gen_label(NULL, "?next", NULL, true);
	char* next2 = gen_label(NULL, "?next", NULL, true);
	char* next3 = gen_label(NULL, "?next", NULL, true);
	char* next4 = gen_label(NULL, "?next", NULL, true);

	if(next1 == NULL || next2 == NULL || next3 == NULL || next4 == NULL){
		free(next1); free(next2); free(next3); free(next4);
		return INTERNAL_ERROR;
	}

	G("POPS GF@_tmp_res");
	G("TYPE GF@_tmp1 GF@_tmp_res");

	G("JUMPIFNEQ %s GF@_tmp string@nil", next1);
	G("		JUMP %s", false_label);	

	G("LABEL %s", next1);	
	G("JUMPIFNEQ %s GF@_tmp string@int", next2);
	G("		JUMPIFEQ %s GF@_tmp_res int@0", false_label);
	G("		JUMP %s", true_label);

	G("LABEL %s", next2);	
	G("JUMPIFNEQ %s GF@_tmp string@float", next3);
	G("		JUMPIFEQ %s GF@_tmp_res float@0.0", false_label);
	G("		JUMP %s", true_label);

	G("LABEL %s", next3);	
	G("JUMPIFNEQ %s GF@_tmp string@string", next4);
	G("		JUMPIFEQ %s GF@_tmp_res string@", false_label);
	G("		JUMP %s", true_label);

	G("LABEL %s", next4);	
	G("		JUMPIFEQ %s GF@_tmp_res bool@false", false_label);
	G("		JUMP %s", true_label);

	free(next1); free(next2); free(next3); free(next4);

	return OK;
}

// OK
error_codes_t gen_while(AST_node_t* while_n, generator_context_t* gen_context){
    AST_node_t* expr_n = while_n->children_arr[0];
    AST_node_t* body_n = while_n->children_arr[1];

	error_codes_t res;
	
	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// get unique label ?while_start, ?while_body_start, ?while_end,
	char *while_start, *while_body_start, *while_end;
	if(gen_while_labels(&while_start, &while_body_start, &while_end) == false) return INTERNAL_ERROR;

	G("LABEL %s", while_start);

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// generate code to jump to ?while_body_start if expression result is true and ?while_end if it is false
	if((res = gen_true_false_jump(while_body_start, while_end)) != OK) return res;

	G("LABEL %s", while_body_start);

	// generate while body
	if((res = gen_body(body_n, gen_context)) != OK) return res;

	G("JUMP %s", while_start);

	G("LABEL %s", while_end);

	free(while_start); free(while_body_start); free(while_end);

    return OK;
}

// OK
error_codes_t gen_if(AST_node_t* if_n, generator_context_t* gen_context){
    AST_node_t* expr_n = if_n->children_arr[0];
    AST_node_t* branch_true_body_n = if_n->children_arr[1];
    AST_node_t* branch_false_body_n = if_n->children_arr[2];

	error_codes_t res;

	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// get unique label ?branch_true, ?branch_false, ?if_end
	char *if_branch_true, *if_branch_false, *if_end;
	if(gen_if_labels(&if_branch_true, &if_branch_false, &if_end) == false) return INTERNAL_ERROR;

	// generate code to jump to branch_true if expression result is true and branch_false if it is false
	if((res = gen_true_false_jump(if_branch_true, if_branch_false)) != OK) return res;

	G("LABEL %s", if_branch_true);

	// generate body of branch_true_body_n
	if((res = gen_body(branch_true_body_n, gen_context)) != OK) return res;

	G("JUMP %s", if_end);

	G("LABEL %s", if_branch_false);

	// generate body of branch_false_body_n
	if((res = gen_body(branch_false_body_n, gen_context)) != OK) return res;

	G("LABEL %s", if_end);

	free(if_branch_true); free(if_branch_false); free(if_end);
    
	return OK;
}

// OK
error_codes_t gen_ass_expr(AST_node_t* ass_expr_n, generator_context_t* gen_context){
    AST_node_t* var_n = ass_expr_n->children_arr[0];
    AST_node_t* expr_n = ass_expr_n->children_arr[1];

	error_codes_t res;

	char* var_name = var_n->data.str;

	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// result of expression is on top of data stack
	// pop result to varaible with name var_name
	// depending on context variable var_name is either global or local variable
	if(gen_context->is_in_function == true)
		G("POPS LF@%s", var_name);
	else
		G("POPS GF@%s", var_name);
	
	return OK;
}

// OK
// Generates code to check if all variable function arguments were defined (initialized)
error_codes_t gen_func_args_def_checks(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	AST_node_t* arg_n;
	char* var_name;
	
	for(size_t i = 0; i < func_call_n->children_count; ++i){
		arg_n = func_call_n->children_arr[i];

		if(arg_n->type == ID_N){
			var_name = arg_n->data.str;

			if((res = gen_var_def_check(var_name, gen_context)) != OK) return res;
		}
	}

	return OK;
}

// OK
error_codes_t gen_func_args_push(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	AST_node_t* arg_n;
	char* var_name;
	char* lit_value;
	
	for(size_t i = 0; i < func_call_n->children_count; ++i){
		arg_n = func_call_n->children_arr[i];

		if(arg_n->type == ID_N){
			var_name = arg_n->data.str;

			if(gen_context->is_in_function == true)
				G("PUSHS LF@%s", var_name);
			else
				G("PUSHS GF@%s", var_name);
		}
		else{
			lit_value = arg_n->data.str;

			if(arg_n->type == STR_LIT_N)
				G("PUSHS string@%s", lit_value);
			else if(arg_n->type == FLT_LIT_N)
				G("PUSHS float@%s", lit_value);
			else if(arg_n->type == INT_LIT_N)
				G("PUSHS int@%s", lit_value);
			else if(arg_n->type == NULL_LIT_N)
				G("PUSHS nil@nil");
			else
				return INTERNAL_ERROR;
		}
	}

	// as last argument push number of arguments passed to function
	size_t arg_count = func_call_n->children_count;

	G("PUSHS int@%zu", arg_count);
	
	return OK;
}

// OK
error_codes_t gen_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	char* func_name = func_call_n->data.str;

	// get unique label ?func_name
	char* func_def_ok_label = gen_label(NULL, "?func_def_ok", NULL, true);
	if(func_def_ok_label == NULL) return INTERNAL_ERROR;

	// check if function was defined
	G("JUMPIFEQ %s GF@?%s?defined bool@true", func_def_ok_label, func_name);
	G("EXIT int@3"); // use of undefined function
	G("LABEL %s", func_def_ok_label);

	free(func_def_ok_label);

	// generate code to check if all variable function arguments were defined (initialized)
	if((res = gen_func_args_def_checks(func_call_n, gen_context)) != OK) return res;

	// generate code to push arguments to stack to be passed to function
	if((res = gen_func_args_push(func_call_n, gen_context)) != OK) return res;

	// call function
	G("CALL %s", func_name);

	return OK;
}

// OK
error_codes_t gen_ass_func(AST_node_t* ass_func_n, generator_context_t* gen_context){
    AST_node_t* var_n = ass_func_n->children_arr[0];
    AST_node_t* func_call_n = ass_func_n->children_arr[1];
	char* var_name = var_n->data.str;

    error_codes_t res;

	// generate function call
	if((res = gen_func_call(func_call_n, gen_context)) != OK) return res;
	
	// result of function call is on top of stack
	// pop function result to varaible with name var_name
	// depending on context variable var_name is either global or local variable
	if(gen_context->is_in_function == true){
		G("POPS LF@%s", var_name);
	}
	else{
		G("POPS GF@%s", var_name);
	}
	
	return OK;
}

// OK
error_codes_t gen_standalone_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	// generate function call
	if((res = gen_func_call(func_call_n, gen_context)) != OK) return res;

	// pop top of stack somewhere, result is not needed
	G("POPS GF@_tmp_res");

	return OK;
}

// OK
error_codes_t gen_standalone_expr(AST_node_t* expr_n, generator_context_t* gen_context){
	error_codes_t res;

	// generate expression variables definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;
	
	// pop top of stack somewhere, result is not needed
	G("POPS GF@_tmp_res");

	return OK;
}

// OK
error_codes_t gen_body(AST_node_t* body_n, generator_context_t* gen_context){
    error_codes_t res = OK;

	AST_node_t* cur_node;
    for(size_t i = 0; i < body_n->children_count; ++i){
		cur_node = body_n->children_arr[i];
		switch(cur_node->type){
			case WHILE_N: 
				res = gen_while(cur_node, gen_context);
				break;
			case IF_N:
				res = gen_if(cur_node, gen_context);
				break;
			case ASS_EXPR_N:
				res = gen_ass_expr(cur_node, gen_context);
				break;
			case ASS_FUNC_N:
				res = gen_ass_func(cur_node, gen_context);
				break;
			case FUNC_CALL_N:
				res = gen_standalone_func_call(cur_node, gen_context);
				break;	
			case EXPR_N:
				res = gen_standalone_expr(cur_node, gen_context);
				break;
			case RETURN_N:
				res = gen_return(cur_node, gen_context);
				break;
			case MISSING_RETURN_N:
				res = gen_missing_return(cur_node, gen_context);
				break;
			default:
				res = INTERNAL_ERROR;
		}

        if(res != OK) return res;
    }

    return OK;
}

error_codes_t gen_type_check_jump(char* var_name, datatype_t expected_type, char* matching_type_label, char* different_type_label){

	G("TYPE GF@_tmp_type %s", var_name);

	if(expected_type == INT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@int", matching_type_label);
		G("JUMP %s", different_type_label);
	}
	else if(expected_type == NINT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@int", matching_type_label);
		G("JUMPIFEQ %s GF@_tmp_type string@nil", matching_type_label);
		G("JUMP %s", different_type_label);
	}
	else if(expected_type == FLT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@float", matching_type_label);
		G("JUMP %s", different_type_label);
	}
	else if(expected_type == NFLT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@float", matching_type_label);
		G("JUMPIFEQ %s GF@_tmp_type string@nil", matching_type_label);
		G("JUMP %s", different_type_label);
	}
	else if(expected_type == STR_T){
		G("JUMPIFEQ %s GF@_tmp_type string@string", matching_type_label);
		G("JUMP %s", different_type_label);
	}
	else if(expected_type == NSTR_T){
		G("JUMPIFEQ %s GF@_tmp_type string@string", matching_type_label);
		G("JUMPIFEQ %s GF@_tmp_type string@nil", matching_type_label);
		G("JUMP %s", different_type_label);
	}
	else{
		return INTERNAL_ERROR;
	}
	return OK;
}


error_codes_t gen_func_params(AST_node_t* params_n){
	error_codes_t res;
	size_t param_count = params_n->children_count;
	
	// get unique ?param_count_ok label
	char* param_count_ok_label = gen_label(NULL, "?param_count_ok", NULL, true);
	if(param_count_ok_label == NULL) return INTERNAL_ERROR;

	// check if first argument passed to function (numer of arguments) is the same as param_count (pop it from data stack and compare)
	G("POPS GF@_tmp1");
	G("JUMPIFEQ %s GF@_tmp1 int@%zu", param_count_ok_label, param_count);
	G("EXIT int@4"); // wrong number of arguments
	G("LABEL %s", param_count_ok_label);

	free(param_count_ok_label);

	AST_node_t* param_n;
	AST_node_t* id_n;
	AST_node_t* type_n;
	
	char* param_name;
	datatype_t param_type;

	for(size_t i = 0; i < params_n->children_count; ++i){
		param_n = params_n->children_arr[i];

		type_n = param_n->children_arr[0];
		id_n = param_n->children_arr[1];

		param_type = type_n->data.type;
		param_name = id_n->data.str;

		char* param_var_name = gen_label("LF@", param_name, NULL, false);
		char* param_type_ok_label = gen_label(NULL, "?param_type_ok?", param_name, false);
		char* param_type_wrong_label = gen_label(NULL, "?param_type_wrong?", param_name, false);
		if(param_type_ok_label == NULL || param_type_wrong_label == NULL || param_var_name == NULL){
			free(param_type_ok_label); free(param_type_wrong_label); free(param_var_name);
			return INTERNAL_ERROR;
		}

		G("DEFVAR LF@%s", param_name);
		G("POPS LF@%s", param_name);

		if((res = gen_type_check_jump(param_var_name, param_type, param_type_ok_label, param_type_wrong_label)) != OK){
			free(param_type_ok_label); free(param_type_wrong_label); free(param_var_name);
			return res;
		}

		G("LABEL %s", param_type_wrong_label);
		G("EXIT int@4");

		G("LABEL %s", param_type_ok_label);

		free(param_type_ok_label); free(param_type_wrong_label); free(param_var_name);
	}

	return OK;
}


error_codes_t gen_func_def(AST_node_t* func_def_n, generator_context_t* gen_context){
    AST_node_t* params_n = func_def_n->children_arr[0];
    //AST_node_t* type_n = func_def_n->children_arr[1];
	AST_node_t* used_variables_n = func_def_n->children_arr[2];
    AST_node_t* body_n = func_def_n->children_arr[3];

	char* func_name = func_def_n->data.str;

	// enter function context
	gen_context->is_in_function = true;

	gen_context->func_end_label = gen_label("?", func_name, "?end", false);
	if(gen_context->func_end_label == NULL) return INTERNAL_ERROR;

	// set function declaration flag to true
	G("MOVE GF@?%s?defined bool@true", func_name);
	
	// generate function header
	G("LABEL %s", func_name);
	G("CREATEFRAME");
	G("PUSHFRAME");

	error_codes_t res;

	// generate code for accepting function params
	if((res = gen_func_params(params_n)) != OK) return res;

	// generate definitions of used variable inside function
	if((res = gen_var_defs(used_variables_n, gen_context)) != OK) return res;

	// generate function body
	if((res = gen_body(body_n, gen_context)) != OK) return res;


	// generate function footer
	G("LABEL %s", gen_context->func_end_label);
	G("POPFRAME");
	G("RETURN");

	// exit function context
	gen_context->is_in_function = false;
	free(gen_context->func_end_label);
	gen_context->func_end_label = NULL;

    return OK;
}

//OK
error_codes_t gen_missing_return(AST_node_t* missing_return_n, generator_context_t* gen_context){
	G("EXIT int@6"); // reached end of non void function, that has not returned anything
	return OK;
}

// OK
error_codes_t gen_return(AST_node_t* return_n, generator_context_t* gen_context){
	// get expression node
	AST_node_t* expr_n = return_n->children_arr[0];

    // get expected return type
    datatype_t expected_return_type = return_n->data.type;

	error_codes_t res;

    if(expected_return_type == VOID_T){
        if(gen_context->is_in_function == true){
			// we are in function
            // generate: return from function, returning null !!!! (void functions return NULL in php)
			G("PUSHS nil@nil");
			G("JUMP %s", gen_context->func_end_label);
        }
        else{
            // we are in main body
			G("EXIT int@0");
        }
    }
	else{
		// generate expression, expression result will be top element of data stack
        if((res = gen_expr(expr_n, gen_context)) != OK) return res;

		if(gen_context->is_in_function == true){
            // we are in function, expected_return_type != -1

			// check if type of expression result on top of data stack is expected type
			char* return_type_wrong_label = gen_label(NULL, "?return_type_wrong", NULL, true);
			if(return_type_wrong_label == NULL){
				free(return_type_wrong_label);
				return INTERNAL_ERROR;
			}

			G("POPS GF@_tmp_res");
			G("PUSHS GF@_tmp_res");

			if((res = gen_type_check_jump("GF@_tmp_res", expected_return_type, gen_context->func_end_label, return_type_wrong_label)) != OK){
				free(return_type_wrong_label);
				return res;
			}

			G("LABEL %s", return_type_wrong_label);
			G("EXIT int@4");
        }
        else{
            // we are in main body
			G("EXIT int@0"); // !!warning do not return result of expression, must call exit, we are in main body
        }
	}

	return OK;
}

// OK
error_codes_t gen_func_def_flags(AST_node_t* used_func_list_n){
    AST_node_t* id_n;
    char* func_name;

    // iterate over every ID node of used_func_list_n to generate definition flags for all functions
    for(size_t i = 0; i < used_func_list_n->children_count; ++i){
        id_n = used_func_list_n->children_arr[i];
        func_name = id_n->data.str;

		G("DEFVAR GF@?%s?defined", func_name);
		G("MOVE GF@?%s?defined bool@false", func_name);
    }

    return OK;
}

bool is_built_in_func(char* func_name){
	if(strcmp(func_name, "reads"))
		return true;
	else if(strcmp(func_name, "readi"))
		return true;
	else if(strcmp(func_name, "readf"))
		return true;
	else if(strcmp(func_name, "write"))
		return true;
	else if(strcmp(func_name, "floatval"))
		return true;
	else if(strcmp(func_name, "intval"))
		return true;
	else if(strcmp(func_name, "strval"))
		return true;
	else if(strcmp(func_name, "strlen"))
		return true;
	else if(strcmp(func_name, "substring"))
		return true;
	else if(strcmp(func_name, "ord"))
		return true;
	else if(strcmp(func_name, "chr"))
		return true;
	else
		return false;
}

// OK
error_codes_t gen_built_in_functions(AST_node_t* used_func_list_n){
    AST_node_t* id_n;
    char* func_name;

    // iterate over every ID node of used_func_list_n to find and generate all built-in functions
    for(size_t i = 0; i < used_func_list_n->children_count; ++i){
        id_n = used_func_list_n->children_arr[i];
        func_name = id_n->data.str;

		if(is_built_in_func(func_name) == true){
			G("MOVE GF@?%s$defined bool@true", func_name);
			// generate built in function //TODO
		}
    }

    return OK;
}

// OK
error_codes_t gen_var_defs(AST_node_t* used_vars_list_n, generator_context_t* gen_context){
    AST_node_t* id_n;
    char* var_name;

    // iterate over every ID node of used_vars_list_n and generate definition of every variable name
    for(size_t i = 0; i < used_vars_list_n->children_count; ++i){
        id_n = used_vars_list_n->children_arr[i];
        var_name = id_n->data.str;

		if(gen_context->is_in_function == true)
			G("DEFVAR LF@%s", var_name);
		else
			G("DEFVAR GF@%s", var_name);
    }

    return OK;
}


// OK
error_codes_t gen_prog(AST_node_t* prog_n, generator_context_t* gen_context){
	if(prog_n->children_count < 2) return INTERNAL_ERROR;

	G(".IFJcode22");

	// generate global temporary variables
	G("DEFVAR GF@_tmp1");
	G("DEFVAR GF@_tmp2");
	G("DEFVAR GF@_tmp_res");
	G("DEFVAR GF@_tmp_type");

    // set context to global, we are in main body
    gen_context->is_in_function = false;

	// get node containing all used function
    AST_node_t* used_func_list_n = prog_n->children_arr[0];

    // generate function definition flags
    gen_func_def_flags(used_func_list_n);

    // generate built in functions
    gen_built_in_functions(used_func_list_n);
    
    // get node containing all used variables in main body
    AST_node_t* used_vars_list_n = prog_n->children_arr[1];

    // generate (global) varaible definitions
    gen_var_defs(used_vars_list_n, gen_context);

	error_codes_t res = OK;

	AST_node_t* cur_node;
    for(size_t i = 2; i < prog_n->children_count; ++i){
		cur_node = prog_n->children_arr[i];
		switch(cur_node->type){
			case BODY_N:
				res = gen_body(cur_node, gen_context);
				break;
			case FUNC_DEF_N:
				res = gen_func_def(cur_node, gen_context);
				break;
			default:
				res = INTERNAL_ERROR;
		}

		if(res != OK) return res;
    }

    return OK;
}

// creates and initializes generator context structure
generator_context_t* generator_context_create(){
    generator_context_t* gen_context = malloc(sizeof(*gen_context));
    if(gen_context == NULL) return NULL;

    gen_context->is_in_function = false;
	gen_context->func_end_label = NULL;

    return gen_context;    
}

// frees generator context structure
void generator_context_free(generator_context_t* gen_context){
    if(gen_context != NULL){
        free(gen_context);
    }
}

void generator(AST_node_t* root){
    // check if generator starting conditions were met
    if(root == NULL){
		error = INTERNAL_ERROR;
		return;
	} 
    if(root->type != PROG_N){
		error = INTERNAL_ERROR;
		return;
	} 
    
    // create generator context
    generator_context_t* gen_context = generator_context_create();
    if(gen_context == NULL){
		error = INTERNAL_ERROR;
		return;
	}

    // run code generator
    error = gen_prog(root, gen_context);

    // free generator context
    generator_context_free(gen_context);
}
