#include "semantic_analyzer.h"
#include "abstract_syntax_tree.h"
#include "error.h"
#include "stdbool.h"
#include "symtable.h"
#include <string.h>

typedef struct{
    AST_node_t* used_func_list_n;
    AST_node_t* used_vars_list_n;

    symbol_info_t* parent_func_symbol_info;

    symtable_t* active_symtable;

    symtable_t* global_symtable;
    symtable_t* local_symtable;
}semantic_context_t;

char* create_string_copy(char* str){
	if(str == NULL) return NULL;

	char* copy = malloc(sizeof(char)*(strlen(str) + 1));

	if(copy != NULL) strcpy(copy, str);
	
	return copy;
}

error_codes_t sem_expr_n(AST_node_t* expr_n, semantic_context_t* sem_context){
	// TODO: semantic check for expression
}

error_codes_t sem_var_n(AST_node_t* ass_var_n, semantic_context_t* sem_context){
	// search of varaible in symtable
	bool name_found;
	symbol_info_t* var_info = symtable_lookup_insert(sem_context->active_symtable, ass_var_n->data.str, &name_found);
	if(var_info == NULL) return INTERNAL_ERROR;

	if(name_found == false){
		AST_node_t* used_var_n = AST_create_add_child(sem_context->used_vars_list_n, ID_N);
		if(used_var_n == NULL) return INTERNAL_ERROR;

		used_var_n->data.str = create_string_copy(ass_var_n->data.str);
		if(used_var_n->data.str == NULL) return INTERNAL_ERROR;
	}

	return OK;
}


error_codes_t sem_while_n(AST_node_t* while_n, semantic_context_t* sem_context){
    AST_node_t* expr_n = while_n->children_arr[0];
    AST_node_t* body_n = while_n->children_arr[1];

	error_codes_t res;

	// check semantics of EXPR node
	if((res = sem_expr_n(expr_n, sem_context)) != OK) return res;

	// check semantics of BODY node
	if((res = sem_body_n(body_n, sem_context)) != OK) return res;

    return OK;
}

error_codes_t sem_if_n(AST_node_t* if_n, semantic_context_t* sem_context){
    AST_node_t* expr_n = if_n->children_arr[0];
    AST_node_t* branch1_body_n = if_n->children_arr[1];
    AST_node_t* branch2_body_n = if_n->children_arr[2];

	error_codes_t res;
    // check semantics of EXPR node
	if((res = sem_expr_n(expr_n, sem_context)) != OK) return res;

	// check semantics of BODY node
	if((res = sem_body_n(branch1_body_n, sem_context)) != OK) return res;

	// check semantics of BODY node
	if((res = sem_body_n(branch2_body_n, sem_context)) != OK) return res;
    
	return OK;
}

error_codes_t sem_ass_expr_n(AST_node_t* ass_expr_n, semantic_context_t* sem_context){
    AST_node_t* var_n = ass_expr_n->children_arr[0];
    AST_node_t* expr_n = ass_expr_n->children_arr[1];
    
	error_codes_t res;

	// check sematnics of ID node
	if((res = sem_var_n(var_n, sem_context)) != OK) return res;

	// check semantics of EXPR node
	if((res = sem_expr_n(expr_n, sem_context)) != OK) return res;

	return OK;
}

error_codes_t sem_ass_func_n(AST_node_t* ass_func_n, semantic_context_t* sem_context){
    AST_node_t* var_n = ass_func_n->children_arr[0];
    AST_node_t* func_call_n = ass_func_n->children_arr[1];

    error_codes_t res;
	
	// check sematnics of ID node
	if((res = sem_var_n(var_n, sem_context)) != OK) return res;

	// check semantics of EXPR node
	if((res = sem_func_call_n(func_call_n, sem_context)) != OK) return res;
    

	return OK;
}

error_codes_t sem_body_n(AST_node_t* body_n, semantic_context_t* sem_context){
    error_codes_t res = OK;

	AST_node_t* cur_node;
    for(size_t i = 0; i < body_n->children_count; ++i){
		cur_node = body_n->children_arr[i];
		switch(cur_node->type){
			case USED_VARS_LIST_N:
				res = OK;
				break;
			case WHILE_N: 
				res = sem_while_n(cur_node, sem_context);
				break;
			case IF_N:
				res = sem_if_n(cur_node, sem_context);
				break;
			case ASS_EXPR_N:
				res = sem_ass_expr_n(cur_node, sem_context);
				break;
			case ASS_FUNC_N:
				res = sem_ass_expr_n(cur_node, sem_context);
				break;
			case FUNC_CALL_N:
				res = sem_func_call_n(cur_node, sem_context);
				break;	
			case RETURN_N:
				res = sem_return_n(cur_node, sem_context);
				break;
			case EXPR_N:
				res = sem_expr_n(cur_node, sem_context);
				break;
			default:
				res = INTERNAL_ERROR;
		}

        if(res != OK) return res;
    }

    return OK;
}

error_codes_t sem_func_args(AST_node_t* func_call_n, semantic_context_t* sem_context){
	error_codes_t res;

	AST_node_t* cur_arg;
	for(size_t i = 0; i < func_call_n->children_count; ++i){
		cur_arg = func_call_n->children_arr[i];

		if(cur_arg->type == ID_N){
			if((res = sem_var_n(cur_arg, sem_context)) != OK) return res;
		}
	}

	return OK;
}

error_codes_t sem_func_call_n(AST_node_t* func_call_n, semantic_context_t* sem_context){
	bool name_found;
	symbol_info_t* func_info = symtable_lookup_insert(sem_context->global_symtable, func_call_n->data.str, &name_found);
    if(func_info == NULL) return INTERNAL_ERROR;

	if(name_found == false){
		AST_node_t* used_func_n = AST_create_insert_child(sem_context->used_func_list_n, 0, ID_N);
		if(used_func_n == NULL) return INTERNAL_ERROR;

		used_func_n->data.str = create_string_copy(func_call_n->data.str);
		if(used_func_n->data.str == NULL) return INTERNAL_ERROR;
	}

	error_codes_t res = OK;

	// semantics for arguments
	if((res = sem_func_args(func_call_n, sem_context)) != OK) return res;

	return OK;
}

// calls function sem_var_n on every param variable
// has to be called after changing into function local context
error_codes_t sem_func_params(AST_node_t* params_n, semantic_context_t* sem_context){
	error_codes_t res;

	AST_node_t* cur_param;
	for(size_t i = 0; i < params_n->children_count; ++i){
		cur_param = params_n->children_arr[i];

		if(cur_param->children_count != 2) return INTERNAL_ERROR;

		// TODO: to add or not to add params to USED_VARS_LIST_N
		if((res = sem_var_n(cur_param->children_arr[1], sem_context)) != OK) return res;
	}

	return OK;
}


error_codes_t sem_func_def_n(AST_node_t* func_def_n, semantic_context_t* sem_context){
    AST_node_t* params_n = func_def_n->children_arr[0];
    AST_node_t* type_n = func_def_n->children_arr[1];
    AST_node_t* body_n = func_def_n->children_arr[2];

    // insert function name to symbol table
	bool name_found;
    symbol_info_t* func_info = symtable_lookup_insert(sem_context->global_symtable, func_def_n->data.str, &name_found);
    if(func_info == NULL) return INTERNAL_ERROR;

    if(name_found == true){
        if(func_info->defined == true){
            // attempted function redefinition
            return SEM_ERROR_3;
        }
    }
    else{
		// function was not found in symtable
		AST_node_t* used_func_n = AST_create_insert_child(sem_context->used_func_list_n, 0, ID_N);
		if(used_func_n == NULL) return INTERNAL_ERROR;

		used_func_n->data.str = create_string_copy(func_def_n->data.str);
		if(used_func_n->data.str == NULL) return INTERNAL_ERROR;
    }

	// set function definition flag
	func_info->defined = true;

	// create new local function USED_VARS_LIST_N
	AST_node_t* new_used_vars_list_n = AST_create_insert_child(body_n, 0, USED_VARS_LIST_N);
	if(new_used_vars_list_n == NULL) return INTERNAL_ERROR;

	// change semantic context when entering function context
	AST_node_t* previous_used_vars_list_n = sem_context->used_vars_list_n;
	sem_context->used_vars_list_n = new_used_vars_list_n;

    sem_context->parent_func_symbol_info = func_info;
    sem_context->active_symtable = sem_context->local_symtable;

	// clear local symtable
	symtable_clear(sem_context->local_symtable);

	error_codes_t res = OK;

	// semantics for params
	if((res = sem_func_params(params_n, sem_context)) != OK) return res;

	// check semantics of function body
    if((res = sem_body_n(body_n, sem_context)) != OK) return res;

    // exit function context
	sem_context->used_vars_list_n = previous_used_vars_list_n;

    sem_context->parent_func_symbol_info = NULL;
    sem_context->active_symtable = sem_context->global_symtable;
	
	// add implicit return nodes to end of function body
	if(func_info->return_type == VOID_T){
		if(AST_create_insert_child(body_n, body_n->children_count, RETURN_N) == NULL) return INTERNAL_ERROR;
	}
	else{
		if(AST_create_insert_child(body_n, body_n->children_count, MISSING_RETURN_N) == NULL) return INTERNAL_ERROR;
	}

    return OK;
}

error_codes_t sem_return_n(AST_node_t* return_n, semantic_context_t* sem_context){
	// get expression node
	AST_node_t* expr_n = return_n->children_arr[0];

	// check if return is in function
    if(sem_context->parent_func_symbol_info != NULL){
		// return is inside function

		// get expected return type of function
		datatype_t* expected_return_type = sem_context->parent_func_symbol_info->return_type;

		// check if function return type is void
		if(expected_return_type == VOID_T){
			// return type is void, expression must be empty
			if(expr_n->data.expression != NULL && expr_n->data.expression->token_count != 0) return SEM_ERROR_6;
		}
		else{
			// return type is not void, expression must not be empty
			if(expr_n->data.expression == NULL) return SEM_ERROR_6;
			else if(expr_n->data.expression->token_count == 0) return SEM_ERROR_6;
		}

		// assign expected return type to return node (to be used by code generator)
		return_n->data.type = expected_return_type;
	}
	else{
		// return is outside function scope, inside main body

		// return type can be any type
		return_n->data.type = -1;
	}

    // check semantics of expression
    return sem_expr_n(expr_n, sem_context);
}

error_codes_t sem_prog_n(AST_node_t* prog_n, semantic_context_t* sem_context){

	// context is global, thus make global_symtable active
    sem_context->active_symtable = sem_context->global_symtable;

	// insert node containing used functions at beginning of program, and add it to semantic context
    AST_node_t* used_func_list_n;
    if((used_func_list_n == AST_create_insert_child(prog_n, 0, USED_FUNC_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }
    sem_context->used_func_list_n = used_func_list_n;

	// insert node containing used varaibles inside main body at beginning of program, and add it to semantic context
    AST_node_t* used_vars_list_n;
    if((used_vars_list_n == AST_create_insert_child(prog_n, 0, USED_VARS_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }
    sem_context->used_vars_list_n = used_vars_list_n;


	error_codes_t res = OK;

	AST_node_t* cur_node;
    for(size_t i = 2; i < prog_n->children_count; ++i){
		cur_node = prog_n->children_arr[i];
		switch(cur_node->type){
			case BODY_N:
				res = sem_body_n(cur_node, sem_context);
				break;
			case FUNC_DEF_N:
				res = sem_func_def_n(cur_node, sem_context);
				break;
			default:
				res = INTERNAL_ERROR;
		}

		if(res != OK) return res;
    }
    return OK;
}

// frees semantic context structure
void semantic_context_free(semantic_context_t* sem_context){
    if(sem_context != NULL){
        symtable_free(sem_context->global_symtable);
        symtable_free(sem_context->local_symtable);
        free(sem_context);
    }
}

// creates and initializes semantic context structure
semantic_context_t* semantic_context_create(){
    semantic_context_t* sem_context = malloc(sizeof(*sem_context));
    if(sem_context == NULL) return NULL;

    sem_context->parent_func_symbol_info = NULL;

    sem_context->active_symtable = NULL;

    sem_context->global_symtable = symtable_create();
    sem_context->local_symtable = symtable_create();

    if(sem_context->global_symtable == NULL || sem_context->local_symtable == NULL){
        free_semantic_context(sem_context);
        return NULL;
    }

    return sem_context;    
}  

void semantic_analyzer(AST_node_t* root){
    // check if semantic analyzer starting conditions were met
    if(root == NULL) error = INTERNAL_ERROR; return;
    if(root->type != PROG_N) error = INTERNAL_ERROR; return;
    
    // create semantic context
    semantic_context_t* sem_context = semantic_context_create();

    // TODO: insert built in functions to global symtable

    // run semantic analysis
    error = sem_prog_n(root, sem_context);

    // free semnatic context
    semantic_context_free(sem_context);
}