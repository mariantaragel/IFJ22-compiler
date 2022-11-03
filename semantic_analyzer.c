#include "semantic_analyzer.h"
#include "error.h"
#include "stdbool.h"
#include "symtable.h"

typedef struct{
    AST_node_t* parent_def_var_list_n;
    symbol_info_t* parent_func_symbol_info;

    symtable_t* global_symtable;
    symtable_t* local_symtable;
}semantic_context_t;

error_codes_t sem_prog_n(AST_node_t* prog_n, semantic_context_t* sem_context){
    error_codes_t res;
    for(size_t i = 0; i < prog_n->children_count; ++i){
        if((res = semantic_crossroads(prog_n->children_arr[i], sem_context)) != OK){
            return res;
        }
    }
    return OK;
}

error_codes_t sem_body_n(AST_node_t* body_n, semantic_context_t* sem_context){
    error_codes_t res;
    for(size_t i = 0; i < body_n->children_count; ++i){
        if((res = semantic_crossroads(body_n->children_arr[i], sem_context)) != OK){
            return res;
        }
    }
    return OK;
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

}

error_codes_t sem_ass_func_n(AST_node_t* ass_func_n, semantic_context_t* sem_context){

}

error_codes_t sem_func_call_n(AST_node_t* func_call_n, semantic_context_t* sem_context){

}

error_codes_t sem_func_def_n(AST_node_t* func_def_n, semantic_context_t* sem_context){
    AST_node_t* params_n = func_def_n->children_arr[0];
    AST_node_t* return_n = func_def_n->children_arr[1];
    AST_node_t* body_n = func_def_n->children_arr[2];

    bool name_found;
    symbol_info_t* function_info;

    function_info = symtable_lookup_insert(sem_context->global_symtable, func_def_n->data.str, &name_found);
    if(function_info == NULL) return INTERNAL_ERROR;
    if(name_found == true) return SEM_ERROR_3; // tried to redefine function

    sem_context->parent_func_symbol_info = function_info;

    // fill info



    // parse body

    // exit function scope
    sem_context->parent_func_symbol_info = NULL;

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

error_codes_t sem_expr_n(AST_node_t* expr_n, semantic_context_t* sem_context){

}

error_codes_t sem_id_n(AST_node_t* id_n, semantic_context_t* sem_context){
    // check if variable is defined ?? maybe unnecessary, will be check in ass_func, ass_
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
		//case ARG_LIST_N:
		//case ARG_N:
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

void semantic_analyzer(AST_node_t* root){
    if(root == NULL) error = INTERNAL_ERROR; return;
    if(root->type != PROG_N) error = INTERNAL_ERROR; return;

    // insert built in functions to symtable

    semantic_context_t* sem_context = malloc(sizeof(*sem_context));
    if(sem_context == NULL) error = INTERNAL_ERROR; return;

    sem_context->parent_def_var_list_n = NULL;
    sem_context->parent_func_symbol_info = NULL;

    error = semantic_crossroads(root, sem_context);
}