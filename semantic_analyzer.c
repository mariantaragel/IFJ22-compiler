#include "semantic_analyzer.h"
#include "abstract_syntax_tree.h"
#include "error.h"
#include "symtable.h"
#include <stdbool.h>
#include <string.h>

typedef struct{
    AST_node_t* used_func_list_n;

    AST_node_t* used_vars_list_n;
	AST_node_t* prev_used_vars_list_n;

    symbol_info_t* parent_func_symbol_info;

    symtable_t* active_symtable;
    symtable_t* global_symtable;
    symtable_t* local_symtable;
}semantic_context_t;

error_codes_t sem_var_name(char* var_name, bool add_to_used_var_list, semantic_context_t* sem_context);
error_codes_t sem_var_n(AST_node_t* ass_var_n, bool add_to_used_var_list, semantic_context_t* sem_context);
error_codes_t sem_expr_n(AST_node_t* expr_n, semantic_context_t* sem_context);
error_codes_t sem_while_n(AST_node_t* while_n, semantic_context_t* sem_context);
error_codes_t sem_if_n(AST_node_t* if_n, semantic_context_t* sem_context);
error_codes_t sem_ass_expr_n(AST_node_t* ass_expr_n, semantic_context_t* sem_context);
error_codes_t sem_ass_func_n(AST_node_t* ass_func_n, semantic_context_t* sem_context);
error_codes_t sem_return_n(AST_node_t* return_n, semantic_context_t* sem_context);

error_codes_t sem_body_n(AST_node_t* body_n, semantic_context_t* sem_context);

error_codes_t sem_func_args(AST_node_t* func_call_n, semantic_context_t* sem_context);
error_codes_t sem_func_call_n(AST_node_t* func_call_n, semantic_context_t* sem_context);

error_codes_t sem_func_params(AST_node_t* params_n, semantic_context_t* sem_context);
error_codes_t sem_enter_func_def_context(AST_node_t* func_def_n, semantic_context_t* sem_context);
error_codes_t sem_exit_func_def_context(semantic_context_t* sem_context);
error_codes_t sem_return_n(AST_node_t* return_n, semantic_context_t* sem_context);
error_codes_t sem_func_def_n(AST_node_t* func_call_n, semantic_context_t* sem_context);

error_codes_t sem_prog_n(AST_node_t* prog_n, semantic_context_t* sem_context);

// frees semantic context structure
void semantic_context_free(semantic_context_t* sem_context){
    if(sem_context != NULL){
		// free symtables
        symtable_free(sem_context->global_symtable);
        symtable_free(sem_context->local_symtable);

		// free sematnic context itself
        free(sem_context);
    }
}

// creates and initializes semantic context structure
semantic_context_t* semantic_context_create(){
	// allocate semantic context
    semantic_context_t* sem_context = malloc(sizeof(*sem_context));
    if(sem_context == NULL) return NULL;

	// initialize semantic context
	sem_context->used_func_list_n = NULL;
	sem_context->used_vars_list_n = NULL;
	sem_context->prev_used_vars_list_n = NULL;

    sem_context->parent_func_symbol_info = NULL;

    sem_context->active_symtable = NULL;

	// create local and global symtables
    sem_context->global_symtable = symtable_create();
    sem_context->local_symtable = symtable_create();

    if(sem_context->global_symtable == NULL || sem_context->local_symtable == NULL){
        semantic_context_free(sem_context);
        return NULL;
    }

	// return allocated semantic context
    return sem_context;
}

void semantic_analyzer(AST_node_t* root){
    // check if semantic analyzer starting conditions were met
    if(root == NULL){
		error = INTERNAL_ERROR;
		return;
	}
		
    if(root->type != PROG_N){
		error = INTERNAL_ERROR;
		return;
	}
    
    // create semantic context
    semantic_context_t* sem_context = semantic_context_create();
	if(sem_context == NULL){
		error = INTERNAL_ERROR;
		return;
	} 
    // run semantic analysis
    error = sem_prog_n(root, sem_context);

    // free semnatic context
    semantic_context_free(sem_context);
}

error_codes_t insert_all_built_in_functions_to_symtable(semantic_context_t* sem_context){
	// built-in functions: reads, readi, readf, write, floatval, intval, strval, strlen, substring, ord, chr
	char built_in_func_names[][10] = {"reads", "readi", "readf", "write", 
	"floatval", "intval", "strval", "strlen", "substring", "ord", "chr"};

	const size_t built_in_func_count = sizeof(built_in_func_names) / sizeof(built_in_func_names[0]);

	// insert all built in functions to symtable and set them as defined
	symbol_info_t* func_info;
	for(size_t i = 0; i < built_in_func_count; ++i){
		if((func_info = symtable_lookup_insert(sem_context->global_symtable, built_in_func_names[i], NULL))== NULL){
			return INTERNAL_ERROR;
		}
		func_info->defined = true;
	}
	
	return OK;
}

error_codes_t sem_prog_n(AST_node_t* prog_n, semantic_context_t* sem_context){
	// context is global, thus make global_symtable active
    sem_context->active_symtable = sem_context->global_symtable;

	// insert node containing used functions at beginning of program, and add it to semantic context
    if((sem_context->used_func_list_n = AST_create_insert_child(prog_n, 0, USED_FUNC_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }

	// insert node containing used variables inside main body at beginning of program, and add it to semantic context
    if((sem_context->used_vars_list_n = AST_create_insert_child(prog_n, 1, USED_VARS_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }

	error_codes_t res = OK;

	if((res = insert_all_built_in_functions_to_symtable(sem_context)) != OK) return res;

	AST_node_t* cur_node;
    for(size_t i = 2; i < prog_n->children_count; ++i){
		cur_node = prog_n->children_arr[i];
		switch(cur_node->type){
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
				res = sem_ass_func_n(cur_node, sem_context);
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

error_codes_t sem_body_n(AST_node_t* body_n, semantic_context_t* sem_context){
    error_codes_t res = OK;

	AST_node_t* cur_node;
    for(size_t i = 0; i < body_n->children_count; ++i){
		cur_node = body_n->children_arr[i];
		switch(cur_node->type){
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
				res = sem_ass_func_n(cur_node, sem_context);
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


char* create_string_copy(char* str){
	if(str == NULL) return NULL;

	char* copy = malloc(sizeof(char)*(strlen(str) + 1));

	if(copy != NULL) strcpy(copy, str);
	
	return copy;
}

error_codes_t sem_var_name(char* var_name, bool add_to_used_var_list, semantic_context_t* sem_context){
	bool name_found;
	symbol_info_t* var_info = symtable_lookup_insert(sem_context->active_symtable, var_name, &name_found);
	if(var_info == NULL) return INTERNAL_ERROR;

	if(name_found == false && add_to_used_var_list == true){
		AST_node_t* used_var_n = AST_create_add_child(sem_context->used_vars_list_n, ID_N);
		if(used_var_n == NULL) return INTERNAL_ERROR;

		used_var_n->data.str = create_string_copy(var_name);
		if(used_var_n->data.str == NULL) return INTERNAL_ERROR;
	}

	return OK;
}

error_codes_t sem_var_n(AST_node_t* ass_var_n, bool add_to_used_var_list, semantic_context_t* sem_context){
	return sem_var_name(ass_var_n->data.str, add_to_used_var_list, sem_context);
}

error_codes_t sem_expr_n(AST_node_t* expr_n, semantic_context_t* sem_context){
	// get expression from expr node data
	token_array_t* exp_array = expr_n->data.expression;
	if(exp_array == NULL) return INTERNAL_ERROR;

	error_codes_t res;

	// call semantics of var name for every variable in token array
	for(size_t i = 0; i < exp_array->token_count; ++i){
		if(exp_array->array[i]->type == VAR_ID){
			if((res = sem_var_name(exp_array->array[i]->aval, true, sem_context)) != OK) return res;
		}
	}

	return OK;
}

error_codes_t sem_while_n(AST_node_t* while_n, semantic_context_t* sem_context){
    AST_node_t* expr_n = while_n->children_arr[0];
    AST_node_t* body_n = while_n->children_arr[1];

	error_codes_t res;

	if((res = sem_expr_n(expr_n, sem_context)) != OK) return res;
	if((res = sem_body_n(body_n, sem_context)) != OK) return res;

    return OK;
}

error_codes_t sem_if_n(AST_node_t* if_n, semantic_context_t* sem_context){
    AST_node_t* expr_n = if_n->children_arr[0];
    AST_node_t* branch1_body_n = if_n->children_arr[1];
    AST_node_t* branch2_body_n = if_n->children_arr[2];

	error_codes_t res;

	if((res = sem_expr_n(expr_n, sem_context)) != OK) return res;
	if((res = sem_body_n(branch1_body_n, sem_context)) != OK) return res;
	if((res = sem_body_n(branch2_body_n, sem_context)) != OK) return res;
    
	return OK;
}

error_codes_t sem_ass_expr_n(AST_node_t* ass_expr_n, semantic_context_t* sem_context){
    AST_node_t* var_n = ass_expr_n->children_arr[0];
    AST_node_t* expr_n = ass_expr_n->children_arr[1];
    
	error_codes_t res;

	if((res = sem_var_n(var_n, true, sem_context)) != OK) return res;
	if((res = sem_expr_n(expr_n, sem_context)) != OK) return res;

	return OK;
}

error_codes_t sem_ass_func_n(AST_node_t* ass_func_n, semantic_context_t* sem_context){
    AST_node_t* var_n = ass_func_n->children_arr[0];
    AST_node_t* func_call_n = ass_func_n->children_arr[1];

    error_codes_t res;
	
	if((res = sem_var_n(var_n, true, sem_context)) != OK) return res;
	if((res = sem_func_call_n(func_call_n, sem_context)) != OK) return res;
    
	return OK;
}

error_codes_t sem_func_args(AST_node_t* func_call_n, semantic_context_t* sem_context){
	error_codes_t res;

	AST_node_t* cur_arg;
	for(size_t i = 0; i < func_call_n->children_count; ++i){
		cur_arg = func_call_n->children_arr[i];

		if(cur_arg->type == ID_N){
			if((res = sem_var_n(cur_arg, true, sem_context)) != OK) return res;
		}
	}

	return OK;
}

error_codes_t sem_func_call_n(AST_node_t* func_call_n, semantic_context_t* sem_context){
	char* func_name = func_call_n->data.str;
	symbol_info_t* func_info = symtable_lookup_insert(sem_context->global_symtable, func_name, NULL);
    if(func_info == NULL) return INTERNAL_ERROR;

	if(func_info->used == false){
		AST_node_t* used_func_n = AST_create_insert_child(sem_context->used_func_list_n, 0, ID_N);
		if(used_func_n == NULL) return INTERNAL_ERROR;

		used_func_n->data.str = create_string_copy(func_name);
		if(used_func_n->data.str == NULL) return INTERNAL_ERROR;

		func_info->used = true;
	}

	error_codes_t res = OK;

	// semantics for arguments
	if((res = sem_func_args(func_call_n, sem_context)) != OK) return res;

	return OK;
}


error_codes_t sem_func_params(AST_node_t* params_n, semantic_context_t* sem_context){
	AST_node_t* cur_param;
	AST_node_t* id_n;

	char* var_name;
	bool name_found;
	symbol_info_t* var_info;

	for(size_t i = 0; i < params_n->children_count; ++i){
		cur_param = params_n->children_arr[i];

		if(cur_param->children_count != 2) return INTERNAL_ERROR;

		id_n = cur_param->children_arr[1];

		var_name = id_n->data.str;

		// insert variable to active symtable
		var_info = symtable_lookup_insert(sem_context->active_symtable, var_name, &name_found);
		if(var_info == NULL) return INTERNAL_ERROR;

		// check if function parameter was not redefined
		if(name_found == true){
			return SEM_ERROR_8; // other semantic error
		}
	}
	return OK;
}

error_codes_t sem_enter_func_def_context(AST_node_t* func_def_n, semantic_context_t* sem_context){
	// insert function name to symbol table
	bool name_found;
    symbol_info_t* func_info = symtable_lookup_insert(sem_context->global_symtable, func_def_n->data.str, &name_found);
    if(func_info == NULL) return INTERNAL_ERROR;

    if(func_info->defined == true){
        // attempted function redefinition
        return SEM_ERROR_3;
    }

	if(func_info->used == false){
		// function was not found in symtable
		AST_node_t* used_func_id_n = AST_create_add_child(sem_context->used_func_list_n, ID_N);
		if(used_func_id_n == NULL) return INTERNAL_ERROR;

		used_func_id_n->data.str = create_string_copy(func_def_n->data.str);
		if(used_func_id_n->data.str == NULL) return INTERNAL_ERROR;
    }

	// set function definition and usage flags
	func_info->defined = true;
	func_info->used = true;

	// create new local function USED_VARS_LIST_N
	AST_node_t* new_used_vars_list_n = AST_create_insert_child(func_def_n, 2, USED_VARS_LIST_N);
	if(new_used_vars_list_n == NULL) return INTERNAL_ERROR;

	// change semantic context when entering function context
	sem_context->prev_used_vars_list_n = sem_context->used_vars_list_n;
	sem_context->used_vars_list_n = new_used_vars_list_n;
    sem_context->parent_func_symbol_info = func_info;
    sem_context->active_symtable = sem_context->local_symtable;

	// clear local symtable
	symtable_clear(sem_context->local_symtable);

	return OK;
}

error_codes_t sem_exit_func_def_context(semantic_context_t* sem_context){
	// exit function context
	sem_context->used_vars_list_n = sem_context->prev_used_vars_list_n;;
    sem_context->parent_func_symbol_info = NULL;
    sem_context->active_symtable = sem_context->global_symtable;

	return OK;
}


error_codes_t sem_func_def_n(AST_node_t* func_def_n, semantic_context_t* sem_context){
    AST_node_t* params_n = func_def_n->children_arr[0];
    AST_node_t* type_n = func_def_n->children_arr[1];
    AST_node_t* body_n = func_def_n->children_arr[2];

	error_codes_t res;

	// semantics for entering function context
	if((res = sem_enter_func_def_context(func_def_n, sem_context)) != OK) return res;

	// add return type to function symbol info
	sem_context->parent_func_symbol_info->return_type = type_n->data.type;

	// semantics of params and function body
	if((res = sem_func_params(params_n, sem_context)) != OK) return res;
    if((res = sem_body_n(body_n, sem_context)) != OK) return res;

	// add implicit return nodes to end of function body
	if(type_n->data.type == VOID_T){
		AST_node_t *return_n;
		if((return_n = AST_create_insert_child(body_n, body_n->children_count, RETURN_N)) == NULL) return INTERNAL_ERROR;
		return_n->data.type = VOID_T;
	}
	else{
		if(AST_create_insert_child(body_n, body_n->children_count, MISSING_RETURN_N) == NULL) return INTERNAL_ERROR;
	}

	// semantics for exiting function definition
	if((res = sem_exit_func_def_context(sem_context)) != OK) return res;
    
    return OK;
}

error_codes_t sem_return_n(AST_node_t* return_n, semantic_context_t* sem_context){
	// get expression node
	AST_node_t* expr_n = return_n->children_arr[0];

	// check if return is in function
    if(sem_context->parent_func_symbol_info != NULL){
		// return is inside function

		// get expected return type of function
		datatype_t expected_return_type = sem_context->parent_func_symbol_info->return_type;

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