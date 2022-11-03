#include "semantic_analyzer.h"
#include "error.h"
#include "stdbool.h"
#include "symtable.h"

typedef struct{
    AST_node_t* parent_def_var_list_n;
    symbol_info_t* parent_func_symbol_info;

    symtable_t* active_symtable;

    symtable_t* global_symtable;
    symtable_t* local_symtable;

    symtable_t* tmp_symtable1, *tmp_symtable2;
}semantic_context_t;

error_codes_t sem_prog_n(AST_node_t* prog_n, semantic_context_t* sem_context){

    sem_context->active_symtable = sem_context->global_symtable;

    error_codes_t res = OK;
    for(size_t i = 0; i < prog_n->children_count; ++i){
        if((res = semantic_crossroads(prog_n->children_arr[i], sem_context)) != OK){
            break;
        }
    }
    return res;
}

error_codes_t sem_body_n(AST_node_t* body_n, semantic_context_t* sem_context){
    error_codes_t res = OK;
    for(size_t i = 0; i < body_n->children_count; ++i){
        if((res = semantic_crossroads(body_n->children_arr[i], sem_context)) != OK){
            break;
        }
    }
    return res;
}

error_codes_t sem_while_n(AST_node_t* while_n, semantic_context_t* sem_context){
    AST_node_t* expr_n = while_n->children_arr[0];
    AST_node_t* body_n = while_n->children_arr[1];

    AST_node_t* def_var_list_n;
    if((def_var_list_n == AST_create_insert_child(while_n, 0, DEF_VAR_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }

    bool is_def_var_list_parent = false;
    if(sem_context->parent_def_var_list_n == NULL){
        sem_context->parent_def_var_list_n = def_var_list_n;
        is_def_var_list_parent = true;
    }

    // do something

    if(is_def_var_list_parent == true){
        sem_context->parent_def_var_list_n = NULL;
    }

    return OK;
}

error_codes_t sem_if_n(AST_node_t* if_n, semantic_context_t* sem_context){
    AST_node_t* expr_n = if_n->children_arr[0];
    AST_node_t* body1_n = if_n->children_arr[1];
    AST_node_t* body2_n = if_n->children_arr[2];

    AST_node_t* def_var_list_n;
    if((def_var_list_n == AST_create_insert_child(if_n, 0, DEF_VAR_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }

    bool is_def_var_list_parent = false;
    if(sem_context->parent_def_var_list_n == NULL){
        sem_context->parent_def_var_list_n = def_var_list_n;
        is_def_var_list_parent = true;
    }


    // do something
    

    if(is_def_var_list_parent == true){
        sem_context->parent_def_var_list_n = NULL;
    }
}

error_codes_t sem_ass_expr_n(AST_node_t* ass_expr_n, semantic_context_t* sem_context){
    AST_node_t* id_n = ass_expr_n->children_arr[0];
    AST_node_t* expr_n = ass_expr_n->children_arr[1];

    AST_node_t* def_var_list_n;
    if((def_var_list_n == AST_create_insert_child(ass_expr_n, 0, DEF_VAR_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }


    bool is_def_var_list_parent = false;
    if(sem_context->parent_def_var_list_n == NULL){
        sem_context->parent_def_var_list_n = def_var_list_n;
        is_def_var_list_parent = true;
    }
    
    // do something
    
    if(is_def_var_list_parent == true){
        sem_context->parent_def_var_list_n = NULL;
    }
}

error_codes_t sem_ass_func_n(AST_node_t* ass_func_n, semantic_context_t* sem_context){
    AST_node_t* id_n = ass_func_n->children_arr[0];
    AST_node_t* func_call_n = ass_func_n->children_arr[1];

    AST_node_t* def_var_list_n;
    if((def_var_list_n == AST_create_insert_child(ass_func_n, 0, DEF_VAR_LIST_N)) == NULL){
        return INTERNAL_ERROR;
    }


    bool is_def_var_list_parent = false;
    if(sem_context->parent_def_var_list_n == NULL){
        sem_context->parent_def_var_list_n = def_var_list_n;
        is_def_var_list_parent = true;
    }


    // do something
    

    if(is_def_var_list_parent == true){
        sem_context->parent_def_var_list_n = NULL;
    }
}

error_codes_t sem_expr_n(AST_node_t* expr_n, semantic_context_t* sem_context){

}

error_codes_t sem_id_n(AST_node_t* id_n, semantic_context_t* sem_context){

}

error_codes_t sem_func_call_n(AST_node_t* func_call_n, semantic_context_t* sem_context){

}

/*
error_codes_t fill_in_func_def_info(symbol_info_t* func_info, AST_node_t *func_n){
    func_info->
    size_t param_count = params_n->children_count;

}

error_codes_t func_info_fill_in
bool func_info_check_declaration(){

}*/


error_codes_t sem_func_def_n(AST_node_t* func_def_n, semantic_context_t* sem_context){
    AST_node_t* params_n = func_def_n->children_arr[0];
    AST_node_t* type_n = func_def_n->children_arr[1];
    AST_node_t* body_n = func_def_n->children_arr[2];

    bool name_found;
    symbol_info_t* func_info;

    // insert function name to symbol table
    func_info = symtable_lookup_insert(sem_context->global_symtable, func_def_n->data.str, &name_found);
    if(func_info == NULL) return INTERNAL_ERROR;

    error_codes_t res = OK;

    if(name_found == true){
        if(func_info->defined == true){
            // attempted function redefinition
            return SEM_ERROR_3;
        }
        else{
            // function call was found in previous
        }
    }
    else{

    }

    // change semantic context when entering function context
    sem_context->parent_func_symbol_info = func_info;
    sem_context->active_symtable = sem_context->local_symtable;



    // parse body

    // exit function context
    sem_context->parent_func_symbol_info = NULL;
    sem_context->active_symtable = sem_context->global_symtable;

    return OK;
}

error_codes_t sem_return_n(AST_node_t* return_n, semantic_context_t* sem_context){
    // if return statement is not outside function definition retrun semantic error 6
    if(sem_context->parent_func_symbol_info == NULL) return SEM_ERROR_6;

    // get function return type and assign it to return node, as expected type of evaluated expression
    return_n->data.type = sem_context->parent_func_symbol_info->return_type;

    // check semantics of expression
    AST_node_t* expr_n = return_n->children_arr[0];
    return sem_expr_n(expr_n, sem_context);
}

error_codes_t semantic_crossroads(AST_node_t* cur_node, semantic_context_t* sem_context){
    error_codes_t res = OK;

    switch(cur_node->type){
        case PROG_N:
            res = sem_prog_n(cur_node, sem_context);
            break;
		case BODY_N:
            res = sem_body_n(cur_node, sem_context);
            break;	
		case WHILE_N: 
            res = sem_while_n(cur_node, sem_context);
            break;
		case IF_N: break;
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
		case FUNC_DEF_N:
            res = sem_func_def_n(cur_node, sem_context);
            break;
        case RETURN_N: break;
            res = sem_return_n(cur_node, sem_context);
            break;
		case EXPR_N: break;
            res = sem_expr_n(cur_node, sem_context);
            break;
        case ID_N:
            res = sem_id_n(cur_node, sem_context);
            break;
        // rest of node list will be checked inside semantic actions listed above
		//case PARAM_LIST_N:
		//case PARAM_N:
		//case TYPE_N:
		//case STR_LIT_N:
		//case INT_LIT_N:
		//case FLT_LIT_N:
		//case NULL_LIT_N:
		//case DEFINE_VAR_N:
		default: res = INTERNAL_ERROR;
    }

    return res;
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

    sem_context->parent_def_var_list_n = NULL;
    sem_context->parent_func_symbol_info = NULL;

    sem_context->active_symtable = NULL;

    sem_context->tmp_symtable1 = NULL;
    sem_context->tmp_symtable2 = NULL;

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
    error = semantic_crossroads(root, sem_context);

    // free semnatic context
    semantic_context_free(sem_context);
}