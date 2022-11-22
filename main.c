/**
 * @name main.c
 * @brief Implementation of compiler main
 * @authors Marián Tarageľ
 * @date 18.11.2022
 */

#include "parser.h"
#include "error.h"
#include "abstract_syntax_tree.h"
#include "semantic_analyzer.h"
#include "generator.h"
#include <stdio.h>

int main()
{
    AST_node_t *root = program();
    //AST_print(root, stdout);
    if (error != OK) {
        AST_free(root);
        error_print(error);
        return error;
    }

    semantic_analyzer(root);
	//AST_print(root, stdout);
	if (error != OK) {
        AST_free(root);
        error_print(error);
        return error;
    }

	generator(root);
	if (error != OK) {
        AST_free(root);
        error_print(error);
        return error;
    }
    
    AST_free(root);
    error_print(error);
    return error;
}