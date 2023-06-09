/**
 * @file abstract_syntax_tree.h
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Declaration of abstract syntax tree node type (AST_node_t) and operations on abstract syntax tree.
 * @date 2022-11-26
 * 
 */


#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include "token_array.h"

#ifndef DATATYPE_T
#define DATATYPE_T
/**
 * @brief Datatype type.
 * 	Its values are synonymous to FLT_T,STR_T,INT_T,VOID_T,NFLT_T,NINT_T,NSTR_T values of token_type_t.
 */
typedef int datatype_t;
#endif

/**
 * @brief Type of node in AST.
 * 
 * Ch_count = children count of specific node type. If count is X, it means that node can have 0-X children.
 * Ch = node types of children of specific node type. If list of children is in { }, it means that the order of children doesnt matter.
 * Data = type of data stored in node.
 * 
 */
typedef enum {
	/**	Ch_count: X | Ch: { FUNC_DEF_N, FUNC_CALL_N, EXPR_N, ASS_EXPR_N, ASS_FUNC_N, IF_N, WHILE_N, RETURN_N }
	* 	Data: None */
	PROG_N,		

	/**	Ch_count: X
	 * 	Ch: { FUNC_CALL_N, EXPR_N, ASS_EXPR_N, ASS_FUNC_N, IF_N, WHILE_N, RETURN_N }
	* 	Data: None */
	BODY_N, 

	/**	Ch_count: 2 | Ch: EXPR_N, BODY_N
	* 	Data: None */
	WHILE_N,

	/**	Ch_count: 3 | Ch: EXPR_N, BODY_N, BODY_N
	* 	Data: None */
	IF_N,

	/** Ch_count: X | Ch: { ID_N, STR_LIT_N, FLT_LIT_N, INT_LIT_N, NULL_LIT_N }
	* 	Data: char* str (function name) */
	FUNC_CALL_N,

	/** Ch_count: 3 | Ch: PARAMS_N, TYPE_N, BODY_N
	* 	Data: char* str (function name) */
	FUNC_DEF_N,

	/** Ch_count: X | Ch: PARAM_N
	* 	Data: None */
	PARAM_LIST_N,
	/** Ch_count: 2 | Ch: TYPE_N, ID_N
	* 	Data: None */
	PARAM_N,

	/** Ch_count: 1 | Ch: EXPR_N
	* 	Data: datatype_t */
	RETURN_N,

	/** Ch_count: 0 | Ch: None
	 *	Data: None
	 */
	MISSING_RETURN_N,

	/** Ch_count: 2 | Ch: ID_N, EXPR_N
	* 	Data: None */
	ASS_EXPR_N,

	/** Ch_count: 2 | Ch: ID_N, FUNC_CALL_N
	* 	Data: None */
	ASS_FUNC_N,

	/** Ch_count: 0 | Ch: None
	* 	Data: token_array_t* */
	EXPR_N,

	/** Ch_count: 0 | Ch: None
	* 	Data: datatype_t */
	TYPE_N,

	/** Ch_count: 0 | Ch: None
	 * 	Data: char* str (variable name) */
	ID_N,

	/** Ch_count: 0 | Ch: None
	* 	Data: char* str (string literal) */
	STR_LIT_N,

	/** Ch_count: 0 | Ch: None
	* 	Data: char* str (string representation of integer value) */
	INT_LIT_N,

	/** Ch_count: 0 | Ch: None
	* 	Data: char* str (string representation of float value) */
	FLT_LIT_N,

	/** Ch_count: 0 | Ch: None
	* 	Data: None */
	NULL_LIT_N,

	/** Ch_count: X | Ch: ID_N
	* 	Data: char* str (variable id name) */
	USED_VARS_LIST_N
}AST_node_type_t;



/**
 * @brief Abstract syntax tree (AST) node structure type.
 * 
 */
typedef struct AST_node{
	AST_node_type_t type; 			// type of AST node

	size_t children_count;			// number of children
	size_t children_arr_size;		// size of chidren array
	struct AST_node ** children_arr;// array of chidren

	// data of AST node
	struct{
		token_array_t* expression;
		char* str;
		datatype_t type;
	}data;
}AST_node_t;

/**
 * @brief Frees all AST nodes including all data stored in them.
 * 
 * @param root Root node of AST == PROG_N type node.
 */
void AST_free(AST_node_t* root);

/**
 * @brief Creates AST node with specified type and initializes it.
 * 
 * @param type Type of AST node to be created.
 * @return AST_node_t* Returns created AST node on success, otherwise NULL is returned.
 */
AST_node_t* AST_create_node(AST_node_type_t type);

/**
 * @brief Creates new node of specified type and adds it to parent node as its child.
 * 
 * @param parent Parent node.
 * @param type Type of new child node to be created and added to parent node.
 * @return AST_node_t* Pointer to created child node on success, otherwise NULL is returned.
 */
AST_node_t* AST_create_add_child(AST_node_t* parent, AST_node_type_t type);

/**
 * @brief Creates new node of specified type and inserts it before specified index to children array of parent node.
 * 
 * @param parent Parent node, to which new child node will be inserted.
 * @param index Index, before which new child node will be inserted. Has to be in range [0; parent->children_count].
 * @param type Type of new child node to be created and inserted to parent node children.
 * @return AST_node_t* Pointer to created and inserted child node on success, otherwise NULL is returned if insertion failed or index is out of range.
 */
AST_node_t* AST_create_insert_child(AST_node_t* parent, size_t index, AST_node_type_t type);

/**
 * @brief Prints AST graphically in text form to selected file.
 * 
 * @param root Root node of AST.
 * @param fp Pointer to file to be printed in.
 */
void AST_print(AST_node_t* root, FILE *fp);

#endif