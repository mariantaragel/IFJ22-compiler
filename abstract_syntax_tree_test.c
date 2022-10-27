#include "abstract_syntax_tree.h"

#include <stdio.h>


int main(){
	AST_node_t* root = AST_create_node(PROG_N);
	if(root == NULL) return -1;

	AST_node_t* new_node;
	if((new_node = AST_create_add_child(root, FUNC_DEF_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if(AST_add_child(root, AST_create_node(FUNC_DEF_N)) != 0){
		AST_free(root);
		return -1;
	}

	if(AST_add_child(root->children_arr[0], AST_create_node(IF_N)) != 0){
		AST_free(root);
		return -1;
	}

	if(AST_add_child(root->children_arr[0], AST_create_node(BODY_N)) != 0){
		AST_free(root);
		return -1;
	}

	if(AST_add_child(root->children_arr[0], AST_create_node(WHILE_N)) != 0){
		AST_free(root);
		return -1;
	}
	AST_print(root, stdout);
	AST_free(root);
}