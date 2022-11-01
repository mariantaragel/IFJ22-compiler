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

	if((new_node = AST_create_add_child(root, FUNC_DEF_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_add_child(root->children_arr[0], IF_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_add_child(root->children_arr[0], BODY_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_add_child(root->children_arr[0], WHILE_N)) == NULL){
		AST_free(root);
		return -1;
	}



	if((new_node = AST_create_insert_child(root->children_arr[1], 1, IF_N)) != NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_insert_child(root->children_arr[1], 0, IF_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_insert_child(root->children_arr[1], 0, BODY_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_insert_child(root->children_arr[1], 2, ASS_FUNC_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_insert_child(root->children_arr[1], 1, ASS_EXPR_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_add_child(root->children_arr[1], BODY_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_add_child(root->children_arr[1], WHILE_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_insert_child(root->children_arr[1], 0, RETURN_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_insert_child(root->children_arr[1], 7, RETURN_N)) == NULL){
		AST_free(root);
		return -1;
	}

	if((new_node = AST_create_insert_child(root->children_arr[1], 9, RETURN_N)) != NULL){
		AST_free(root);
		return -1;
	}

	AST_print(root, stdout);
	AST_free(root);
}