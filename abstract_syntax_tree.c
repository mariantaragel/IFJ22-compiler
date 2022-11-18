#include "abstract_syntax_tree.h"
#include "token_array.h"
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
	token_array_free(root->data.expression);
	free(root->data.str);

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
	new_node->data.expression = NULL;
	new_node->data.str = NULL;
	new_node->data.type = -1;

	// return created node
	return new_node;
}

int _AST_double_children_array_size(AST_node_t* parent){
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

	return 0;
}


AST_node_t* AST_create_insert_child(AST_node_t* parent, size_t index, AST_node_type_t type){
	if(parent == NULL) return NULL;
	if(index > parent->children_count) return NULL;

	// if children array is full try to double children array size
	if(parent->children_count == parent->children_arr_size){
		if(_AST_double_children_array_size(parent) != 0) return NULL;
	}

	// create new node
	AST_node_t* new_child = AST_create_node(type);
	if(new_child == NULL) return NULL;

	// create space at index position for new child
	for(size_t i = parent->children_count; i != index; --i){
		parent->children_arr[i] = parent->children_arr[i-1];
	}

	// add new child to specified index
	parent->children_arr[index] = new_child;
	++(parent->children_count);

	return new_child;
}

AST_node_t* AST_create_add_child(AST_node_t* parent, AST_node_type_t type){
	if(parent == NULL) return NULL;

	// insert new child at the end of children array
	return AST_create_insert_child(parent, parent->children_count, type);
}

void _AST_print_node(AST_node_t* node, FILE* fp){
	// get string representation of AST node type
	char* node_name;
	switch(node->type){
		case PROG_N: 			node_name = "PROG_N"; break;
		case BODY_N: 			node_name = "BODY_N"; break;	
		case WHILE_N: 			node_name = "WHILE_N"; break;
		case IF_N: 				node_name = "IF_N"; break;	
		case FUNC_CALL_N:		node_name = "FUNC_CALL_N"; break;	
		case FUNC_DEF_N:		node_name = "FUNC_DEF_N"; break;
		case PARAM_LIST_N:		node_name = "PARAM_LIST_N"; break;
		case PARAM_N:			node_name = "PARAM_N"; break;
		case RETURN_N:			node_name = "RETURN_N"; break;
		case ASS_EXPR_N:		node_name = "ASS_EXPR_N"; break;
		case ASS_FUNC_N:		node_name = "ASS_FUNC_N"; break;
		case EXPR_N:			node_name = "EXPR_N"; break;
		case TYPE_N:			node_name = "TYPE_N"; break;
		case ID_N:				node_name = "ID_N"; break;
		case STR_LIT_N:			node_name = "STR_LIT_N"; break;
		case INT_LIT_N:			node_name = "INT_LIT_N"; break;
		case FLT_LIT_N:			node_name = "FLT_LIT_N"; break;
		case NULL_LIT_N:		node_name = "NULL_LIT_N"; break;
		case USED_VARS_LIST_N:	node_name = "USED_VARS_LIST_N"; break;
		case USED_FUNC_LIST_N:	node_name = "USED_FUNC_LIST_N"; break;
		case MISSING_RETURN_N:	node_name = "MISSING_RETURN_N"; break;
		default: 				node_name = "UNKNOWN";
	}
	
	// print node type
	fprintf(fp,"%s",node_name);

	// print node data
	if(node->type == EXPR_N){
		fprintf(fp," [ ");
		token_array_expr_print(node->data.expression, fp);
		fprintf(fp," ]");
	}
	else if(node->type == ID_N || node->type == FUNC_CALL_N || node->type == FUNC_DEF_N || node->type == STR_LIT_N || node->type == FLT_LIT_N || node->type == INT_LIT_N){
		fprintf(fp," { ");
		if(node->data.str != NULL){
			fprintf(fp,"%s", node->data.str);
		}
		fprintf(fp," }");
	}
	else if(node->type == NULL_LIT_N){
		fprintf(fp," { null }");
	}
}

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

	_AST_print_node(root, fp);

	for(size_t i = 0; i < root->children_count; ++i){
		_AST_print(root->children_arr[i], (depth+1), fp);
	}
}

void AST_print(AST_node_t* root, FILE *fp){
	_AST_print(root, 0, fp);
	printf("\n");
}