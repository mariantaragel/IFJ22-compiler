#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdlib.h>  // size_t
#include <stdbool.h> // bool

/////////////////
// Symbol type:

typedef const char * symbol_name_t;

typedef enum {VAR, FUNC} symbol_type_t;

typedef struct{
    symbol_type_t symbol_type;
    bool defined;
}symbol_info_t;

typedef struct {
    symbol_name_t name;
    symbol_info_t info;
} symbol_t;


/////////////////
// Symbol table type:

typedef struct symtable symtable_t;


/////////////////
// Operations:

// Constructor: allocates and initialises new symtable_t structure
// On success pointer to allocated symbol table is returned, otherwise NULL is returned.
symtable_t *symtable_create();

// Destructor: frees all symbols in symbol table "t" and symbol table structure itself.
void symtable_free(symtable_t *t);

// Frees all symbols in symbol table.
void symtable_clear(symtable_t *t);

// Returns number of symbols in symtable.
size_t symtable_get_symbol_count(symtable_t* t);

// Searches symbol table "t" for symbol with specified "name".
// If found, function returns pointer to corresponding symbol, otherwise NULL is returned.
symbol_t * symtable_lookup(symtable_t * t, symbol_name_t name);

// Searches symbol table "t" for symbol with specified "name".
// If found, function returns pointer to corresponding symbol and sets "name_found" to true.
// If symbol was not found, new symbol with specified "name" is created, initialized and added to symbol table "t". 
// Afterwards pointer to newly added symbol is returned and "name_found" is set to false.
symbol_t * symtable_lookup_insert(symtable_t * t, symbol_name_t name, bool *name_found);

#endif