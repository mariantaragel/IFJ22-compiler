/**
 * @file semantic_analyzer.h
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Semantic analyzer interface.
 * @date 2022-11-26
 * 
 */

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "abstract_syntax_tree.h"

void semantic_analyzer(AST_node_t* root);

#endif