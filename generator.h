/**
 * @file generator.h
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Code generator interface.
 * @date 2022-11-26
 * 
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "abstract_syntax_tree.h"

/**
 * @brief Generates IFJcode22 program source code based on semantically analyzed abstract syntax tree and prints it to stdout.
 * If an internal error occured (allocation error, invalid AST structure, ...) global error variable is set to INTERNAL_ERROR.
 * Otherwise on success global error variable is set to OK.
 * 
 * @param root Root node(PROG_N) of semantically analyzed abstract syntax tree.
 */
void generator(AST_node_t* root);

#endif