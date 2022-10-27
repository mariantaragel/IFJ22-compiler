#include "abstract_syntax_tree.h"
#include <stdio.h>


void AST_free(AST_node_t* root){
	if(root == NULL) return;

	// free all children
	for(size_t i = 0; i < root->children_count; ++i){
		AST_free(root->children_arr[i]);
	}

	// free children array
	free(root->children_arr);

	// free data
	if(root->data.expression_token_array != NULL){
		token_array_free(root->data.expression_token_array);
	}
	else if(root->data.str != NULL){
		free(root->data.str);
	}

	// free root itself
	free(root);
}


AST_node_t* AST_create_node(AST_node_type_t type){
	// allocate new node
	AST_node_t* new_node = malloc(sizeof(*new_node));
	if(new_node == NULL) return NULL;

	// init children array
	new_node->children_arr = NULL;
	new_node->children_arr_size = 0;
	new_node->children_count = 0;

	// init type
	new_node->type = type;

	// init data
	new_node->data.expression_token_array = NULL;
	new_node->data.str = NULL;

	// return created node
	return new_node;
}


int AST_add_child(AST_node_t* parent, AST_node_t* new_child){
	if(parent == NULL || new_child == NULL){
		return -1;
	}

	// calculate new size of children array
	size_t new_size;
	if(parent->children_arr_size == 0){
		new_size = 1;
	}
	else{
		new_size = parent->children_arr_size * 2;
	}

	// realloc children array
	AST_node_t** tmp_children_array = parent->children_arr;
	parent->children_arr = realloc(parent->children_arr, sizeof(*(parent->children_arr)) * new_size);
	if(parent->children_arr == NULL){
		parent->children_arr = tmp_children_array;
		return -1;
	}

	// update size
	parent->children_arr_size = new_size;

	// add new child
	parent->children_arr[parent->children_count] = new_child;
	++(parent->children_count);

	// return success
	return 0;
}

AST_node_t* AST_create_add_child(AST_node_t* parent, AST_node_type_t type){
	if(parent == NULL) return NULL;

	// create new node
	AST_node_t* new_child = AST_create_node(type);
	if(new_child == NULL) return NULL;

	// add new node as child node to parent
	if(AST_add_child(parent, new_child) != 0){
		free(new_child);
		return NULL;
	}
	
	// return created new node
	return new_child;
}

/*
typedef struct{
	AST_node_t node_type;
	char *str;
}AST_type_name_pair_t;

static const AST_type_name_pair_t type_name_pair_array[] = {
	{PROG_N, "PROG_N"}, {BODY_N, "BODY_N"}, {WHILE_N, "WHILE_N"}, {IF_N, "IF_N"},
	{FUNC_CALL_N, "FUNC_CALL_N"}, {FUNC_DEF_N, "FUNC_DEF_N"}, {PARAM_LIST_N, "PARAM_LIST_N"}, {PARAM_N, "PARAM_N"}, 
	{RETURN_N, "RETURN_N"}, {ASS_EXPR_N, "ASS_EXPR_N"}, {ASS_FUNC_N, "ASS_FUNC_N"}, 
	{EXPR_N, "EXPR_N"}, {TYPE_N, "TYPE_N"}, {ID_N, "ID_N"},
	{STR_LIT_N, "STR_LIT_N"}, {INT_LIT_N, "INT_LIT_N"}, {FLT_LIT_N, "FLT_LIT_N"}
};

char* _AST_get_type_str(AST_node_type_t node_type){
	size_t type_name_pair_array_size = sizeof(type_name_pair_array) / sizeof(type_name_pair_array[0]);
	for(size_t i = 0; i < type_name_pair_array_size; ++i){
		if(type_name_pair_array[i].node_type == node_type){
			return type
		}

	}
}
*/

void _AST_print(AST_node_t* root, size_t depth, FILE* fp){
	if(depth > 0){
		fprintf(fp, "\n");
		for(size_t i = 0; i < depth-1; ++i){
			fprintf(fp, "     ");
		}
		for(size_t i = 0; i < depth; ++i){
			fprintf(fp, "   ");
		}
		fprintf(fp, "+----");
	}

	char* node_name;
	switch(root->type){
		case PROG_N: 		node_name = "PROG_N"; break;
		case BODY_N: 		node_name = "BODY_N"; break;	
		case WHILE_N: 		node_name = "WHILE_N"; break;
		case IF_N: 			node_name = "IF_N"; break;	
		case FUNC_CALL_N:	node_name = "FUNC_CALL_N"; break;	
		case FUNC_DEF_N:	node_name = "FUNC_DEF_N"; break;
		case PARAM_LIST_N:	node_name = "PARAM_LIST_N"; break;
		case PARAM_N:		node_name = "PARAM_N"; break;
		case RETURN_N:		node_name = "RETURN_N"; break;
		case ASS_EXPR_N:	node_name = "ASS_EXPR_N"; break;
		case ASS_FUNC_N:	node_name = "ASS_FUNC_N"; break;
		case EXPR_N:		node_name = "EXPR_N"; break;
		case TYPE_N:		node_name = "TYPE_N"; break;
		case ID_N:			node_name = "ID_N"; break;
		case STR_LIT_N:		node_name = "STR_LIT_N"; break;
		case INT_LIT_N:		node_name = "INT_LIT_N"; break;
		case FLT_LIT_N:		node_name = "FLT_LIT_N"; break;
		default: node_name = "UNKNOWN";
	}
	
	fprintf(fp,"%s",node_name);

		
	for(size_t i = 0; i < root->children_count; ++i){
		_AST_print(root->children_arr[i], (depth+1), fp);
	}
}

void AST_print(AST_node_t* root, FILE *fp){
	_AST_print(root, 0, fp);
	printf("\n");
}