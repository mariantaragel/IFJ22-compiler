/**
 * @file symtable.h
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Symbol table interface.
 * @date 2022-11-26
 * 
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdlib.h>  // size_t
#include <stdbool.h> // bool
#include "abstract_syntax_tree.h"

#ifndef DATATYPE_T
#define DATATYPE_T
/**
 * @brief Datatype type.
 * 	Its values are synonymous to FLT_T,STR_T,INT_T,VOID_T,NFLT_T,NINT_T,NSTR_T values of token_type_t.
 * 	Additionaly if value is equal to -1, it means that datatype is "any datatype".
 */
typedef int datatype_t;
#endif

/**
 * @brief Symbol information structure data type.
 *
 */
typedef struct{
	bool all_nested_calls_defined;	// flag indicating, that all called functions, from inside this function, were all defined
	
	AST_node_t* func_body_n;		// AST node of function body

	datatype_t return_type;			// return type of function
	
} symbol_info_t;


/////////////////


/**
 * @brief Data type of symbol name.
 */
typedef const char * symbol_name_t;

/**
 * @brief Data type of symbol table structure.
 * 
 */
typedef struct symtable symtable_t;


/////////////////
// Operations:


/**
 * @brief Construcor: allocates and initialises new symtable_t structure.
 * 
 * @return symtable_t* On success pointer to allocated symbol table is returned, otherwise NULL is returned.
 */
symtable_t *symtable_create();


/**
 * @brief Destructor: frees all symbols in symbol table and symbol table structure itself.
 * 
 * @param t Symbol table to be freed.
 */
void symtable_free(symtable_t *t);


/**
 * @brief Frees all symbols in symbol table.
 * 
 * @param t Symbol table to have elements freed.
 */
void symtable_clear(symtable_t *t);

/**
 * @brief Searches symbol table for symbol with specified name and if found, returns pointer to corresponding symbol.
 * 
 * @param t Symbol table to be searched in.
 * @param name Name of symbol to be searched.
 * @return symbol_info_t* If symbol name is found, function returns pointer to corresponding symbol info, otherwise NULL is returned.
 */
symbol_info_t * symtable_lookup(symtable_t * t, symbol_name_t name);


/**
 * @brief Searches symbol table for symbol with specified name.
 * If found, function returns pointer to corresponding symbol info and sets name_found flag to true.
 * If symbol was not found, new symbol with specified name is created, initialized and added to symbol table. name_found is set to false.
 * 
 * If name_found is NULL, then function will not attempt to store any value in it.
 * 
 * @param t Symbol table to be searched in or to which new symbol will be added.
 * @param name Name of symbol to be searched for or name of symbol to be added to symtable.
 * @param name_found Indicator flag is set to true if symbol already existsts, otherwise flag is set to false.
 * @return symbol_info_t* Returns either found or newly inserted symbol info on success, otherwise, if error occured, NULL is returned.
 */
symbol_info_t * symtable_lookup_insert(symtable_t * t, symbol_name_t name, bool *name_found);

#endif