/**
 * @file generator.c
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Implementation of code generator.
 * @date 2022-11-26
 * 
 */


#include "abstract_syntax_tree.h"
#include "error.h"
#include "generator_tools.h"
#include "generator_builtin.h"
#include "generator.h"
#include "expgen.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


error_codes_t gen_var_def_check(char* var_name, generator_context_t* gen_context);
error_codes_t gen_expr_vars_def_checks(AST_node_t* expr_n, generator_context_t* gen_context);
error_codes_t gen_expr(AST_node_t* expr_n, generator_context_t* gen_context);
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
error_codes_t gen_func_params(AST_node_t* params_n, char* func_name);
error_codes_t gen_func_def(AST_node_t* func_def_n, generator_context_t* gen_context);
error_codes_t gen_missing_return();
error_codes_t gen_return(AST_node_t* return_n, generator_context_t* gen_context);
void gen_var_defs(AST_node_t* used_vars_list_n, generator_context_t* gen_context);
error_codes_t gen_prog(AST_node_t* prog_n, generator_context_t* gen_context);
generator_context_t* generator_context_create();
void generator_context_free(generator_context_t* gen_context);

// OK
error_codes_t gen_var_def_check(char* var_name, generator_context_t* gen_context){
	// get unique label ?var_name?var_init_ok
	char* var_init_ok_label = gen_label("?", var_name, "?var_init_ok", true);
	if(var_init_ok_label == NULL) return INTERNAL_ERROR;

	// check whether variable with var_name was initialised
	if(gen_context->is_in_function == true){
		G("TYPE GF@_tmp LF@%s", var_name);
	}
	else{
		G("TYPE GF@_tmp GF@%s", var_name);
	}

	G("JUMPIFNEQ %s GF@_tmp string@", var_init_ok_label);
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
			var_name = expr->array[i]->aval;

			if((res = gen_var_def_check(var_name, gen_context)) != OK) return res;
		}
	}

	return OK;
}

error_codes_t gen_expr(AST_node_t* expr_n, generator_context_t* gen_context){
	// generate code to evaluate expression, including type checks
	// expression result should be at the top of data stack
	gen_expression(expr_n->data.expression, gen_context);

	return OK;
}

// OK
error_codes_t gen_while(AST_node_t* while_n, generator_context_t* gen_context){
	G("# WHILE START");
	inc_ind();

    AST_node_t* expr_n = while_n->children_arr[0];
    AST_node_t* body_n = while_n->children_arr[1];

	error_codes_t res;
	
	G("# CHECK WHILE COND VARS DEFS START");
	inc_ind();

	// generate expression variables definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	dec_ind();
	G("# CHECK WHILE COND VARS DEFS END\n");

	// get unique label ?while_start, ?while_body_start, ?while_end,
	char *while_start, *while_body_start, *while_end;
	if(gen_while_labels(&while_start, &while_body_start, &while_end) == false) return INTERNAL_ERROR;

	G("LABEL %s", while_start);

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	G("CALL &to_bool");
	G("POPS GF@_tmp_res");
	G("JUMPIFEQ %s GF@_tmp_res bool@false", while_end);

	// generate while body
	if((res = gen_body(body_n, gen_context)) != OK) return res;

	G("JUMP %s", while_start);

	G("LABEL %s", while_end);

	free(while_start); free(while_body_start); free(while_end);

	dec_ind();
	G("# WHILE END\n");
    return OK;
}

// OK
error_codes_t gen_if(AST_node_t* if_n, generator_context_t* gen_context){
	G("# IF START");
	inc_ind();

    AST_node_t* expr_n = if_n->children_arr[0];
    AST_node_t* branch_true_body_n = if_n->children_arr[1];
    AST_node_t* branch_false_body_n = if_n->children_arr[2];

	error_codes_t res;

	G("# CHECK IF COND VARS DEFS START");
	inc_ind();

	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	dec_ind();
	G("# CHECK IF COND VARS DEFS END\n");

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// get unique label ?branch_true, ?branch_false, ?if_end
	char *if_branch_true, *if_branch_false, *if_end;
	if(gen_if_labels(&if_branch_true, &if_branch_false, &if_end) == false) return INTERNAL_ERROR;

	G("CALL &to_bool");
	G("POPS GF@_tmp_res");
	G("JUMPIFEQ %s GF@_tmp_res bool@false", if_branch_false);

	// generate body of branch_true_body_n
	if((res = gen_body(branch_true_body_n, gen_context)) != OK) return res;

	G("JUMP %s", if_end);

	G("LABEL %s", if_branch_false);

	// generate body of branch_false_body_n
	if((res = gen_body(branch_false_body_n, gen_context)) != OK) return res;

	G("LABEL %s", if_end);

	free(if_branch_true); free(if_branch_false); free(if_end);
    
	dec_ind();
	G("# IF END\n");
	return OK;
}

// OK
error_codes_t gen_ass_expr(AST_node_t* ass_expr_n, generator_context_t* gen_context){
	G("# EXPR ASS START");
	inc_ind();

    AST_node_t* var_n = ass_expr_n->children_arr[0];
    AST_node_t* expr_n = ass_expr_n->children_arr[1];

	error_codes_t res;

	char* var_name = var_n->data.str;

	G("# CHECK EXPR VARS DEFS START");
	inc_ind();

	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	dec_ind();
	G("# CHECK EXPR VARS DEFS END\n");
	

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// result of expression is on top of data stack
	// pop result to varaible with name var_name
	// depending on context variable var_name is either global or local variable
	if(gen_context->is_in_function == true)
		G("POPS LF@%s", var_name);
	else
		G("POPS GF@%s", var_name);
	
	dec_ind();
	G("# EXPR ASS END\n");
	return OK;
}

// OK
// Generates code to check if all variable function arguments were defined (initialized)
error_codes_t gen_func_args_def_checks(AST_node_t* func_call_n, generator_context_t* gen_context){
	G("# CHECK FUNC ARG INIT START");
	inc_ind();

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

	dec_ind();
	G("# CHECK FUNC ARG INIT END\n");
	return OK;
}

// OK
error_codes_t gen_func_args_push(AST_node_t* func_call_n, generator_context_t* gen_context){
	AST_node_t* arg_n;
	char* var_name;
	char* lit_value;

	size_t arg_count = func_call_n->children_count;
	
	for(size_t i = arg_count; i > 0; --i){
		arg_n = func_call_n->children_arr[i-1];

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
	G("PUSHS int@%zu", arg_count);
	
	return OK;
}

// OK
error_codes_t gen_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	char* func_name = func_call_n->data.str;

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
	G("# FUNC ASS START");
	inc_ind();

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
	
	dec_ind();
	G("# FUNC ASS END\n");
	return OK;
}

// OK
error_codes_t gen_standalone_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	G("# STANDALONE FUNC CALL START");
	inc_ind();

	error_codes_t res;

	// generate function call
	if((res = gen_func_call(func_call_n, gen_context)) != OK) return res;

	// pop top of stack somewhere, result is not needed
	G("POPS GF@_tmp_res");

	dec_ind();
	G("# STANDALONE FUNC CALL END\n");
	return OK;
}

// OK
error_codes_t gen_standalone_expr(AST_node_t* expr_n, generator_context_t* gen_context){
	G("# STANDALONE EXPRESSION START");
	inc_ind();

	error_codes_t res;

	G("# CHECK EXPR VARS DEFS START");
	inc_ind();

	// generate expression variables definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	dec_ind();
	G("# CHECK EXPR VARS DEFS END\n");
	

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;
	
	// pop top of stack somewhere, result is not needed
	G("POPS GF@_tmp_res");

	dec_ind();
	G("# STANDALONE EXPRESSION END\n");
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
				res = gen_missing_return();
				break;
			default:
				res = INTERNAL_ERROR;
		}

        if(res != OK) return res;
    }

    return OK;
}

error_codes_t gen_type_check_jump(char* var_name, datatype_t expected_type, char* type_ok_label){
	G("TYPE GF@_tmp_type %s", var_name);

	if(expected_type == INT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@int", type_ok_label);
	}
	else if(expected_type == NINT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@int", type_ok_label);
		G("JUMPIFEQ %s GF@_tmp_type string@nil", type_ok_label);
	}
	else if(expected_type == FLT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@float", type_ok_label);
	}
	else if(expected_type == NFLT_T){
		G("JUMPIFEQ %s GF@_tmp_type string@float", type_ok_label);
		G("JUMPIFEQ %s GF@_tmp_type string@nil", type_ok_label);
	}
	else if(expected_type == STR_T){
		G("JUMPIFEQ %s GF@_tmp_type string@string", type_ok_label);
	}
	else if(expected_type == NSTR_T){
		G("JUMPIFEQ %s GF@_tmp_type string@string", type_ok_label);
		G("JUMPIFEQ %s GF@_tmp_type string@nil", type_ok_label);
	}
	else{
		return INTERNAL_ERROR;
	}

	return OK;
}


error_codes_t gen_func_params(AST_node_t* params_n, char* func_name){
	G("# FUNCTION PARAMS START");
	inc_ind();

	error_codes_t res;
	size_t param_count = params_n->children_count;
	
	// get unique ?arg_count_ok label
	char* arg_count_ok_label = gen_label("?", func_name, "?arg_count_ok", false);
	if(arg_count_ok_label == NULL) return INTERNAL_ERROR;

	G("# CHECK ARG COUNT START");
	inc_ind();

	// check if first argument passed to function (numer of arguments) is the same as param_count (pop it from data stack and compare)
	G("POPS GF@_tmp");
	G("JUMPIFEQ %s GF@_tmp int@%zu", arg_count_ok_label, param_count);
	G("EXIT int@4"); // wrong number of arguments
	G("LABEL %s", arg_count_ok_label);

	dec_ind();
	G("# CHECK ARG COUNT END\n");

	free(arg_count_ok_label);

	AST_node_t* param_n;
	AST_node_t* id_n;
	AST_node_t* type_n;
	
	char* param_name;
	datatype_t param_type;

	G("# INIT PARAMS, CHECK PARAM TYPES START");
	inc_ind();
	for(size_t i = 0; i < param_count; ++i){
		param_n = params_n->children_arr[i];

		type_n = param_n->children_arr[0];
		id_n = param_n->children_arr[1];

		param_type = type_n->data.type;
		param_name = id_n->data.str;

		char* tmp_func_name = gen_label("?", func_name, "?", false);
		if(tmp_func_name == NULL) return INTERNAL_ERROR;
		char* param_type_ok_label = gen_label(tmp_func_name, param_name, "?param_type_ok", false);
		char* param_var_name = gen_label("LF@", param_name, NULL, false);
		if(param_type_ok_label == NULL || param_var_name == NULL){
			free(param_type_ok_label);  free(param_var_name); free(tmp_func_name);
			return INTERNAL_ERROR;
		}

		G("DEFVAR LF@%s", param_name);
		G("POPS LF@%s", param_name);

		if((res = gen_type_check_jump(param_var_name, param_type, param_type_ok_label)) != OK){
			free(param_type_ok_label); free(param_var_name); free(tmp_func_name);
			return res;
		}
		G("EXIT int@4");

		G("LABEL %s", param_type_ok_label);

		free(param_type_ok_label); free(param_var_name); free(tmp_func_name);
	}
	dec_ind();
	G("# INIT PARAMS, CHECK PARAM TYPES END\n");

	dec_ind();
	G("# FUNCTION PARAMS END");
	return OK;
}

error_codes_t gen_func_def(AST_node_t* func_def_n, generator_context_t* gen_context){
	G("# FUNC [%s] DEF START", func_def_n->data.str);
	inc_ind();

    AST_node_t* params_n = func_def_n->children_arr[0];
    //AST_node_t* type_n = func_def_n->children_arr[1];
	AST_node_t* used_variables_n = func_def_n->children_arr[2];
    AST_node_t* body_n = func_def_n->children_arr[3];

	char* func_name = func_def_n->data.str;

	// enter function context
	gen_context->is_in_function = true;

	gen_context->func_end_label = gen_label("?", func_name, "?end", false);
	if(gen_context->func_end_label == NULL) return INTERNAL_ERROR;
	
	// generate function header
	G("LABEL %s", func_name);
	G("CREATEFRAME");
	G("PUSHFRAME");

	error_codes_t res;

	// generate code for accepting function params
	if((res = gen_func_params(params_n, func_name)) != OK) return res;

	// generate definitions of used variable inside function
	gen_var_defs(used_variables_n, gen_context);

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

	dec_ind();
	G("# FUNC [%s] DEF END\n", func_def_n->data.str);
    return OK;
}

//OK
error_codes_t gen_missing_return(){
	G("# MISSING RETURN START");
	G("\tEXIT int@4"); // reached end of non void function, that has not returned anything
	G("# MISSING RETURN END\n");
	return OK;
}

// OK
error_codes_t gen_return(AST_node_t* return_n, generator_context_t* gen_context){
	G("# RETURN START");
	inc_ind();

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
		// get expression node
		AST_node_t* expr_n = return_n->children_arr[0];
		
		// generate expression variables definition checks
		if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

		// generate expression, expression result will be top element of data stack
        if((res = gen_expr(expr_n, gen_context)) != OK) return res;

		if(gen_context->is_in_function == true){
            // we are in function, expected_return_type != -1

			// check if type of expression result on top of data stack is expected type
			G("POPS GF@_tmp_res");
			G("PUSHS GF@_tmp_res");

			if((res = gen_type_check_jump("GF@_tmp_res", expected_return_type, gen_context->func_end_label)) != OK){
				return res;
			}
			G("EXIT int@4");
        }
        else{
            // we are in main body
			G("EXIT int@0"); // !!warning do not return result of expression, must call exit, we are in main body
        }
	}

	dec_ind();
	G("# RETURN END\n");
	return OK;
}

// OK
void gen_built_in_functions(){
	G("# BUILT IN FUNCTIONS START");
	G(BUILTIN_READS);
	G(BUILTIN_READI);
	G(BUILTIN_READF);
	G(BUILTIN_WRITE);
	G(BUILTIN_FLOATVAL);
	G(BUILTIN_INTVAL);
	G(BUILTIN_STRVAL);
	G(BUILTIN_STRLEN);
	G(BUILTIN_SUBSTRING);
	G(BUILTIN_ORD);
	G(BUILTIN_CHR);
	G("# BUILT IN FUNCTIONS END\n");
}

error_codes_t gen_user_functions(AST_node_t* prog_n, generator_context_t* gen_context){
	G("# USER FUNCTIONS START");
	inc_ind();

	error_codes_t res;

	AST_node_t* cur_node;
	for(size_t i = 1; i < prog_n->children_count; ++i){
		cur_node = prog_n->children_arr[i];
		if(cur_node->type == FUNC_DEF_N){
			if((res = gen_func_def(cur_node, gen_context)) != OK) return res;
		}
    }

	dec_ind();
	G("# USER FUNCTIONS END\n");
	return OK;
}

// OK
void gen_var_defs(AST_node_t* used_vars_list_n, generator_context_t* gen_context){
	G("# DEFINE USED VARS START");
	inc_ind();

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

	dec_ind();
	G("# DEFINE USED VARS END\n");
}

void gen_helper_functions(){
	G("# HELPER FUNCTIONS START");
	G(HELPER_TO_BOOL_);
	gen_operation_functions();
	G("# HELPER FUNCTIONS END\n");
}

void gen_tmp_vars(){
	G("# TMP VARS START");
	inc_ind();
	G("DEFVAR GF@_tmp");
	G("DEFVAR GF@_tmp_res");
	G("DEFVAR GF@_tmp_type");
	G("DEFVAR GF@_arg_count");
	
	G("DEFVAR GF@_rhs");
	G("DEFVAR GF@_lhs");
	G("DEFVAR GF@_tlhs");
	G("DEFVAR GF@_trhs");
	G("DEFVAR GF@_aux1");
	dec_ind();
	G("# TMP VARS END\n");
}

// OK
error_codes_t gen_prog(AST_node_t* prog_n, generator_context_t* gen_context){
	if(prog_n->children_count < 1) return INTERNAL_ERROR;

	// initialise indentation to zero
	ind = 0;

	G(".IFJcode22");

	// generate global temporary variables
	gen_tmp_vars();
	
    // set context to global, we are in main body
    gen_context->is_in_function = false;
    
    // get node containing all used variables in main body
    AST_node_t* used_vars_list_n = prog_n->children_arr[0];

    // generate (global) variable definitions
    gen_var_defs(used_vars_list_n, gen_context);

	error_codes_t res = OK;

	AST_node_t* cur_node;
    for(size_t i = 1; i < prog_n->children_count; ++i){
		cur_node = prog_n->children_arr[i];
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
			case FUNC_DEF_N:
				// skip
				res = OK;
				break;
			default:
				res = INTERNAL_ERROR;
		}

		if(res != OK) return res;
    }

	// end of body
	G("EXIT int@0\n");

	// generate user functions
	if((res = gen_user_functions(prog_n, gen_context)) != OK) return res;

	// generate built in functions
    gen_built_in_functions();

	// generate helper functions
	gen_helper_functions();

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
