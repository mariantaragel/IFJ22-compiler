#include "semantic_analyzer.h"
#include "abstract_syntax_tree.h"
#include "error.h"
#include <stdbool.h>
#include <string.h>

typedef struct{
    bool is_in_function;
}generator_context_t;

error_codes_t gen_expr_vars_def_checks(AST_node_t* expr_n, generator_context_t* gen_context){
	// for every variable in expression generate code to check whether variable was defined (by comparing its flag)

	return OK;
}

error_codes_t gen_expr(AST_node_t* expr_n, generator_context_t* gen_context){
	// generate code to evaluate expression, including type checks
	// expression result should be at the top of data stack

	return OK;
}

error_codes_t gen_while(AST_node_t* while_n, generator_context_t* gen_context){
    AST_node_t* expr_n = while_n->children_arr[0];
    AST_node_t* body_n = while_n->children_arr[1];

	error_codes_t res;
	
	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// get unique label L_WHILE_START, L_WHILE_END

	// generate label L_WHILE_START

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// generate code to check if expression result is true or false
	// 		if expression result is false generate jump to L_WHILE_END
	//			probably use JUMPIFsomething, otherwise another label is needed to skip this jump to L_WHILE_END

	// generate while body
	if((res = gen_body(body_n, gen_context)) != OK) return res;

	// generate jump to label L_WHILE_START

	// generate label L_WHILE_END

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

	// get unique label L_IF_BRANCH_FALSE, L_IF_END (have to be unique in whole program)

	// generate code to check if expression result is true or false
	// 		if expression result is false generate jump to L_IF_BRANCH_FALSE
	//			probably use JUMPIFsomething, otherwise another label(L_IF_BRANCH_TRUE) is needed

	// generate body of branch1
	if((res = gen_body(branch1_body_n, gen_context)) != OK) return res;

	// generate jump to L_IF_END

	// generate label L_IF_BRANCH_FALSE

	// generate body of branch2
	if((res = sem_body(branch2_body_n, gen_context)) != OK) return res;

	// generate label L_IF_END
    
	return OK;
}

error_codes_t gen_ass_expr(AST_node_t* ass_expr_n, generator_context_t* gen_context){
    AST_node_t* var_n = ass_expr_n->children_arr[0];
    AST_node_t* expr_n = ass_expr_n->children_arr[1];
    
	error_codes_t res;

	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// generate pop of expression result to variable with name var_n->data.str

	// generate instruction to set definition of variable with name var_n->data.str to true

	return OK;
}

error_codes_t gen_func_args_def_checks(AST_node_t* func_call_n, generator_context_t* gen_context){
	// generate code to check if all varaible function arguments were defined (or initialized)

	return OK;
}

error_codes_t gen_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	// generate code to check function definition flag
	
	// generate code to check if all varaible function arguments were defined (or initialized)
	if((res = gen_func_args_def_checks(func_call_n, gen_context)) != OK) return res;

	// generate code to push arguments to stack to be passed to function

	// generate call of function with name func_call_n->dada.str

	// result of function call is on top of stack

	return OK;
}

error_codes_t gen_ass_func(AST_node_t* ass_func_n, generator_context_t* gen_context){
    AST_node_t* var_n = ass_func_n->children_arr[0];
    AST_node_t* func_call_n = ass_func_n->children_arr[1];

    error_codes_t res;

	// generate function call
	if((res = gen_func_call(func_call_n, gen_context)) != OK) return res;
	// result of function call is on top of stack

	// generate pop of function result to varaible with name var_n->data.str

	// generate instruction to set varaible name as defined (initialized)    

	return OK;
}

error_codes_t gen_standalone_func_call(AST_node_t* func_call_n, generator_context_t* gen_context){
	error_codes_t res;

	// generate function call
	if((res = gen_func_call(func_call_n, gen_context)) != OK) return res;
	// result of function call is on top of stack

	// pop top of stack somewhere, result is not needed

	return OK;
}

error_codes_t gen_standalone_expr(AST_node_t* expr_n, generator_context_t* gen_context){
	error_codes_t res;

	// generate expression varaibles definition checks
	if((res = gen_expr_vars_def_checks(expr_n, gen_context)) != OK) return res;

	// generate expression, expression result will be top element of data stack
	if((res = gen_expr(expr_n, gen_context)) != OK) return res;

	// pop top of stack somewhere, result is not needed

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
				res = gen_ass_exp(cur_node, gen_context);
				break;
			case ASS_FUNC_N:
				res = gen_ass_func(cur_node, gen_context);
				break;
			case FUNC_CALL_N:
				res = gen_standalone_func_call(cur_node, gen_context);
				break;	
			case EXPR_N:
				res = sem_standalone_expr(cur_node, gen_context);
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

// calls function sem_var_n on every param variable
// has to be called after changing into function local context
error_codes_t gen_func_params(AST_node_t* params_n){
	//TODO

	return OK;
}


error_codes_t gen_func_def(AST_node_t* func_def_n, generator_context_t* gen_context){
    AST_node_t* params_n = func_def_n->children_arr[0];
    AST_node_t* type_n = func_def_n->children_arr[1];
	AST_node_t* used_variables_n = func_def_n->children_arr[2];
    AST_node_t* body_n = func_def_n->children_arr[3];

    //TODO

	error_codes_t res;

	if((res = gen_func_params(used_variables_n)) != OK) return res;

	if((res = gen_var_defs(used_variables_n)) != OK) return res;


    return OK;
}

error_codes_t gen_missing_return(AST_node_t* missing_return_n, generator_context_t* gen_context){
    // generate EXIT 6 - reached end of non void function, that has not returned anything
}


error_codes_t gen_return(AST_node_t* return_n, generator_context_t* gen_context){
	// get expression node
	AST_node_t* expr_n = return_n->children_arr[0];

    // get expected return type
    datatype_t* expected_return_type = return_n->data.type;

	error_codes_t res;

    if(expected_return_type != VOID_T){
        // generate expression, expression result will be top element of data stack
        if((res = gen_expr(expr_n, gen_context)) != OK) return res;
    }
    // else expression is empty - dont generate expreesion (garanteed by semantic analyzator)

    if(expected_return_type == VOID_T){
        if(gen_context->is_in_function == true){
            // generate return from function, returning null !!!! (void functions return NULL in php)
        }
        else{
            // we are in main body
            // generate EXIT 0
        }
    }
    else if(expected_return_type == -1){// we are definitely in main body, gen_context->is_in_function is 100% set to true, no need to check
        // we are in main body, return type can be any type
        // no need to check expression result type
        // generate instruction to EXIT 0 - !!warning do not return result of expression, must call exit, we are in main body
    }
    else{
        // we are not in main body and expected_return_tpe != VOID_T
        // generate check for type of expression result on top of data stack
        // if type doesnt match
        //      generate EXIT 4 - wrong type of returning expression
        // else
        //      generate return of expression result
    }
}

error_codes_t gen_func_def_flags(AST_node_t* used_func_list_n){
    AST_node_t* id_n;
    char* func_name;
    // iterate over every ID node of used_func_list_n
    // ID nodes contain names of used functions in .data.str
    for(size_t i = 0; i < used_func_list_n->children_count; ++i){
        id_n = used_func_list_n->children_arr[i];
        func_name = id_n->data.str;
        // generate flag for every function definition and generate instruction to set it to false
    }

    return OK;
}

error_codes_t gen_built_in_functions(AST_node_t* used_func_list_n){
    AST_node_t* id_n;
    char* func_name;
    // iterate over every ID node of used_func_list_n to find and generate all built in function
    for(size_t i = 0; i < used_func_list_n->children_count; ++i){
        id_n = used_func_list_n->children_arr[i];
        func_name = id_n->data.str;
        // if func_name is built in function
        //      generate instruction to set function definition of built in function to true
        //      generate built in function
    }

    return OK;
}

error_codes_t gen_var_defs(AST_node_t* used_vars_list_n){
    AST_node_t* id_n;
    char* var_name;
    // iterate over every ID node of used_vars_list_n and generate definition flag for every variable name
    for(size_t i = 0; i < used_vars_list_n->children_count; ++i){
        id_n = used_vars_list_n->children_arr[i];
        var_name = id_n->data.str;

        // generate flag for definition of variable with name var_name
    }

    return OK;
}



error_codes_t gen_prog(AST_node_t* prog_n, generator_context_t* gen_context){
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

    // generate varaible definition flags
    gen_var_defs(used_vars_list_n);

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

// frees generator context structure
void generator_context_free(generator_context_t* gen_context){
    if(gen_context != NULL){
        free(gen_context);
    }
}

// creates and initializes generator context structure
generator_context_t* generator_context_create(){
    generator_context_t* gen_context = malloc(sizeof(*gen_context));
    if(gen_context == NULL) return NULL;

    gen_context->is_in_function = false;

    return gen_context;    
}  

void generator(AST_node_t* root){
    // check if generator starting conditions were met
    if(root == NULL) error = INTERNAL_ERROR; return;
    if(root->type != PROG_N) error = INTERNAL_ERROR; return;
    
    // create generator context
    generator_context_t* gen_context = generator_context_create();
    if(gen_context == NULL) error = INTERNAL_ERROR; return;

    // run semantic analysis
    error = gen_prog(root, gen_context);

    // free generator context
    generator_context_free(gen_context);
}