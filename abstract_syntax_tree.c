#include "abstract_syntax_tree.h"


void AST_free(AST_node_t* root){
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
	AST_node_t* tmp_children_array = parent->children_arr;
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