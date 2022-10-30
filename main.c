/**
 * @name main.c
 * @brief Implementation of compiler main
 * @authors Marián Tarageľ
 * @date 30.10.2022
 */

#include "parser.h"
#include "error.h"
#include "abstract_syntax_tree.h"
#include <stdio.h>

int main()
{
    AST_node_t *root = program();
    AST_print(root, stdout);
    if (error == OK) {
        printf("OK\n");
    }
    else {
        fprintf(stderr, "ERROR\n");
    }
}