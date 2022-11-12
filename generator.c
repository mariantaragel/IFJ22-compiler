#include "abstract_syntax_tree.h"
#include "error.h"
#include <stdbool.h>
#include <string.h>

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


error_codes_t gen_var_def_check(char* var_name, generator_context_t* gen_context){
	// get unique label ?var_initialised

	// check whether variable with var_name was initialised
	if(gen_context->is_in_function == true){
		// generate: TYPE GF@_tmp LF@var_name
	}
	else{
		// generate: TYPE GF@_tmp GF@var_name
	}

	// generate: JUMPIFEQ ?var_initialised GF@_tmp string@
	// generate: EXIT int@5 // use of undefined variable
	// generate: LABEL ?var_initialised

	return OK;
}


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


error_codes_t gen_true_false_jump(char* true_label, char* false_label){
	// generate: 

	// POPS GF@_tmp_res

	// TYPE GF@_tmp GF@_tmp_res

	// JUMPIFNEQ ?next1 GF@_tmp string@nil
	//		JUMP false_label
	// LABEL ?next1
	// JUMPIFNEQ ?next2 GF@_tmp string@int
	//		JUMPIFEQ false_label GF@_tmp_res int@0
	//		JUMP true_label
	// LABEL ?next2
	// JUMPIFNEQ ?next3 GF@_tmp string@float
	//		JUMPIFEQ false_label GF@_tmp_res float@0.0
	//		JUMP true_label
	// LABEL ?next3
	// JUMPIFNEQ ?next4 GF@_tmp string@string
	//		JUMPIFEQ false_label GF@_tmp_res string@
	//		JUMP true_label
	// LABEL ?next4
	//		JUMPIFEQ false_label GF@_tmp_res bool@false
	//		JUMP true_label

	return OK;
}


error_codes_t gen_while(AST_node_t* while_n, generator_context_t* gen_context){
    AST_node_t* expr_n = while_n->children_arr[0];
    AST_node_t* body_n = while_n->children_arr[1];

	error_codes_t res;
	
	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// get unique label ?while_start, ?while_body_start, ?while_end,
	char *while_start, *while_body_start, *while_end;

	// generate: LABEL ?while_start

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// generate code to jump to ?while_start if expression result is true and ?while_end if it is false
	if((res = gen_true_false_jump(while_body_start, while_end)) != OK) return res;
	
	// generate while body
	if((res = gen_body(body_n, gen_context)) != OK) return res;

	// generate: JUMP while_start

	// generate: LABEL while_end

    return OK;
}


error_codes_t gen_if(AST_node_t* if_n, generator_context_t* gen_context){
    AST_node_t* expr_n = if_n->children_arr[0];
    AST_node_t* branch1_body_n = if_n->children_arr[1];
    AST_node_t* branch2_body_n = if_n->children_arr[2];

	error_codes_t res;

	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// get unique label ?branch_true, ?branch_false, ?if_end
	char *branch_true, *branch_false, *if_end;

	// generate code to jump to branch_true if expression result is true and branch_false if it is false
	if((res = gen_true_false_jump(branch_true, branch_false)) != OK) return res;

	// generate: LABEL branch_true

	// generate body of branch1
	if((res = gen_body(branch1_body_n, gen_context)) != OK) return res;

	// generate: JUMP if_end

	// generate: LABEL branch_false

	// generate body of branch2
	if((res = gen_body(branch2_body_n, gen_context)) != OK) return res;

	// generate: LABEL if_end
    
	return OK;
}


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
	if(gen_context->is_in_function == true){
		// POPS GF@var_name
	}
	else{
		// POPS LF@var_name
	}
	
	return OK;
}

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


error_codes_t gen_func_args_push(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	AST_node_t* arg_n;
	char* var_name;
	char* lit_value;
	
	for(size_t i = 0; i < func_call_n->children_count; ++i){
		arg_n = func_call_n->children_arr[i];

		if(arg_n->type == ID_N){
			var_name = arg_n->data.str;

			if(gen_context->is_in_function == true){
				// generate: PUSHS GF@var_name
			}
			else{
				// generate: PUSHS LF@var_name
			}
		}
		else{
			lit_value = arg_n->data.str;

			if(arg_n->type == STR_LIT_N){
				// generate: PUSHS string@lit_value
			}
			else if(arg_n->type == FLT_LIT_N){
				// generate: PUSHS float@lit_value
			}
			else if(arg_n->type == INT_LIT_N){
				// generate: PUSHS int@lit_value
			}
			else if(arg_n->type == NULL_LIT_N){
				// generate: PUSHS nil@nil
			}
			else{
				return INTERNAL_ERROR;
			}
		}
	}

	// as last argument push number of arguments passed to function
	size_t arg_count = func_call_n->children_count;
	// char* arg_count_str = unsigned_to_string(arg_count)
	// generate: PUSHS int@arg_count_str

	return OK;
}


error_codes_t gen_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	char* func_name = func_call_n->data.str;

	// get unique label ?func_declared

	// generate code to check if function was declared
	// generate: JUMPIFEQ ?func_declared GF@?func_name$declared bool@true
	// generate: EXIT int@3 // use of undefined function
	// generate: LABEL ?func_declared
	
	// generate code to check if all variable function arguments were defined (initialized)
	if((res = gen_func_args_def_checks(func_call_n, gen_context)) != OK) return res;

	// generate code to push arguments to stack to be passed to function
	if((res = gen_func_args_push(func_call_n, gen_context)) != OK) return res;

	// generate: CALL func_name

	return OK;
}


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
		// POPS GF@var_name
	}
	else{
		// POPS LF@var_name
	}
	
	return OK;
}


error_codes_t gen_standalone_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	// generate function call
	if((res = gen_func_call(func_call_n, gen_context)) != OK) return res;

	// result of function call is on top of stack
	// pop top of stack somewhere, result is not needed
	// generate: POPS GF@_tmp_res

	return OK;
}


error_codes_t gen_standalone_expr(AST_node_t* expr_n, generator_context_t* gen_context){
	error_codes_t res;

	// generate expression variables definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;
	
	// pop top of stack somewhere, result is not needed
	// generate: POPS GF@_tmp_res

	return OK;
}


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


error_codes_t gen_func_params(AST_node_t* params_n){
	size_t param_count = params_n->children_count;
	
	// get unique label starting ?param_count_ok e.g. ?param_count_ok_13
	// or maybe something like ?func_name_param_count

	// generate: check if first argument passed to function (numer of arguments) is the same as param_count (pop it from data stack and compare)
	// POPS GF@_tmp1
	// JUMPIFEQ ?param_count_ok GF@_tmp1 int@param_count
	// EXIT int@4 // wrong number of arguments
	// LABEL ?param_count_ok


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

		// generate: DEFVAR LF@param_name
		// generate: POPS LF@param_name
		// generate: check if parameter has same type as param_type
		//		if it is not the same
		//			generate: EXIT int@4

	}

	return OK;
}


error_codes_t gen_func_def(AST_node_t* func_def_n, generator_context_t* gen_context){
    AST_node_t* params_n = func_def_n->children_arr[0];
    AST_node_t* type_n = func_def_n->children_arr[1];
	AST_node_t* used_variables_n = func_def_n->children_arr[2];
    AST_node_t* body_n = func_def_n->children_arr[3];

	char* func_name = func_def_n->data.str;

	// enter function context
	gen_context->is_in_function = true;
	// gen_context->func_end_label = func_name$end

	// set function declaration flag to true
	// generate: MOVE GF@?func_name$declared bool@true

	// generate: LABEL func_name
	// generate: CREATEFRAME
	// generate: PUSHFRAME


	error_codes_t res;

	// generate code for accepting function params
	if((res = gen_func_params(params_n)) != OK) return res;

	// generate definitions of used variable inside function
	if((res = gen_var_defs(used_variables_n, gen_context)) != OK) return res;

	// generate function body
	if((res = gen_body(used_variables_n, gen_context)) != OK) return res;


	// generate: LABEL: gen_context->func_end_label
	// generate: POPFRAME
	// generate: RETURN


	// exit function context
	gen_context->is_in_function = false;
	gen_context->func_end_label = NULL;

    return OK;
}


error_codes_t gen_missing_return(AST_node_t* missing_return_n, generator_context_t* gen_context){
    // generate: EXIT int@6 - reached end of non void function, that has not returned anything

	return OK;
}



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
			//			 PUSHS nil@nil
			//			 JUMP gen_context->func_end_label
        }
        else{
            // we are in main body
            // generate: EXIT int@0
        }
    }
	else{
		// generate expression, expression result will be top element of data stack
        if((res = gen_expr(expr_n, gen_context)) != OK) return res;

		if(gen_context->is_in_function == true){
            // we are in function, expected_return_type != -1

			// check if type of expression result on top of data stack is expected type
			// generate:
			
			// POPS GF@_tmp_res
			// TYPE GF@_tmp GF@_tmp_res

			if(expected_return_type == INT_T){
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@int
				// EXIT int@4
			}
			else if(expected_return_type == NINT_T){
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@int
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@nil
				// EXIT int@4
			}
			else if(expected_return_type == FLT_T){
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@float
				// EXIT int@4 
			}
			else if(expected_return_type == NFLT_T){
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@float
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@nil
				// EXIT int@4
			}
			else if(expected_return_type == STR_T){
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@string
				// EXIT int@4 
			}
			else if(expected_return_type == NSTR_T){
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@string
				// JUMPIFEQ gen_context->func_end_label GF@_tmp string@nil
				// EXIT int@4
			}
			else{
				return INTERNAL_ERROR;
			}
        }
        else{
            // we are in main body
            // generate: EXIT int@0 - !!warning do not return result of expression, must call exit, we are in main body
        }
	}
}


error_codes_t gen_func_def_flags(AST_node_t* used_func_list_n){
    AST_node_t* id_n;
    char* func_name;

    // iterate over every ID node of used_func_list_n to generate definition flags for all functions
    for(size_t i = 0; i < used_func_list_n->children_count; ++i){
        id_n = used_func_list_n->children_arr[i];
        func_name = id_n->data.str;

		// generate: DEFVAR GF@?func_name$declared
		// generate: MOVE GF@?func_name$declared bool@false
    }

    return OK;
}


error_codes_t gen_built_in_functions(AST_node_t* used_func_list_n){
    AST_node_t* id_n;
    char* func_name;

    // iterate over every ID node of used_func_list_n to find and generate all built-in functions
    for(size_t i = 0; i < used_func_list_n->children_count; ++i){
        id_n = used_func_list_n->children_arr[i];
        func_name = id_n->data.str;

        // if func_name is built-in function
        //      generate: MOVE GF@?func_name$declared bool@true
        //      generate: built-in function body
    }

    return OK;
}


error_codes_t gen_var_defs(AST_node_t* used_vars_list_n, generator_context_t* gen_context){
    AST_node_t* id_n;
    char* var_name;

    // iterate over every ID node of used_vars_list_n and generate definition of every variable name
    for(size_t i = 0; i < used_vars_list_n->children_count; ++i){
        id_n = used_vars_list_n->children_arr[i];
        var_name = id_n->data.str;

		if(gen_context->is_in_function == true){
			// generate: DEFVAR LF@var_name
		}
		else{
			// generate: DEFVAR GF@var_name
		}
    }

    return OK;
}



error_codes_t gen_prog(AST_node_t* prog_n, generator_context_t* gen_context){
	if(prog_n->children_count < 2) return INTERNAL_ERROR;

	// generate: .IFJcode22

	// generate: global pomocne promenne

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
