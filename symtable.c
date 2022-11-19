#include <stdlib.h>  // malloc, free, NULL, size_t
#include <string.h>  // strcmp, memcpy, strlen
#include <stdbool.h> // bool
#include <stdint.h>  // uint32_t

#include "symtable.h"

// Maximum average length of symbol table lists.
// Average length = (number_of_symbols_in_symbol_table / symbol_table_bucket_size)
// If average length exceds AVG_LEN_MAX symbol table is resized
#define AVG_LEN_MAX 2

// Initial/default size of symbol table after symtable_init and symtable_clear
#define INITIAL_SIZE 32

//============================

// TODO: add source
// Sdbm implementation of hash function (public-domain). Returns hash of "str".
size_t hash_function(const char* str) {
    uint32_t h = 0;
    const unsigned char* p;
    for (p = (const unsigned char*)str; *p != '\0'; ++p){
        h = 65599 * h + *p;
    }
    return h;
}

//============================

// Symbol table item type and structure
typedef struct symtable_item{
    symbol_info_t symbol_info; 	// symbol info
	symbol_name_t name;			// symbol name
    struct symtable_item *next; // pointer to next item
}symtable_item_t;

// Symbol table structure
struct symtable{
    size_t symbol_count;        // number of symbols in symbol table
    size_t arr_size;            // size of arr_ptr
    symtable_item_t **arr_ptr;  // array of pointers to lists of symbol table items
};

//============================

// Item constructor: creates and initializes symbol table item with specified name.
// Returns pointer to allocated symtable_item_t structure, otherwise NULL is returned.
symtable_item_t *symtable_item_create(symbol_name_t name){
    // create new item
	symtable_item_t *new_item = malloc(sizeof(symtable_item_t));
	if(new_item == NULL) return NULL;

	// create copy of name
	const size_t name_len = strlen(name) + 1;
	char *name_copy = malloc(name_len * sizeof(*name_copy));
	if(name_copy == NULL){
		free(new_item);
		return NULL;
	}
	memcpy(name_copy, name, name_len);

	// initialize new item // TODO: initialize symbol info
	new_item->name = name_copy;
	new_item->symbol_info.used = false;
	new_item->symbol_info.defined = false;
	new_item->symbol_info.return_type = -1;
	

    return new_item;
}

// Item descructor: frees symbol table item
void symtable_item_free(symtable_item_t* item){
    free((char*)(item->name));

    free(item);
}

//============================

// Constructor: allocates and initialises new symtable_t structure
// On success pointer to allocated symbol table is returned, otherwise NULL is returned.
symtable_t *symtable_create(){
    // allocate symtable structure
    symtable_t * t = malloc(sizeof(symtable_t));
    if(t == NULL) return NULL;

    // allocate symtable arr_ptr
    t->arr_ptr = malloc(INITIAL_SIZE * sizeof(symtable_item_t*));
    if(t->arr_ptr == NULL){
        free(t);
        return NULL;
    }

    // initialize arr_ptr elements to NULL
    for(size_t i = 0; i < INITIAL_SIZE; ++i) t->arr_ptr[i] = NULL;

    // initialize arr_size and size to default values
    t->arr_size = INITIAL_SIZE;
    t->symbol_count = 0;

    //return created structure
    return t;
}

//============================

// Returns number of symbols in symtable.
size_t symtable_get_symbol_count(symtable_t* t){
    if(t == NULL) return 0;
    else return t->symbol_count;
}

//============================

// Adds "new_item" to hash table "t".
// Warning: This function is only to be used by symtable_resize. Item to be added has to be unique in hash table.
void _symtable_resize_add(symtable_t *t, symtable_item_t* new_item) {
    // name of new_item to be added
    symbol_name_t name = new_item->name;

    // calculate symtable index
    const size_t symtable_index = hash_function(name) % t->arr_size;

    // get item at symtable_index
    symtable_item_t* cur_item = t->arr_ptr[symtable_index];
    symtable_item_t* prev_item = NULL;

    // iterate trough list until correct position for "new_item" was found
    while (cur_item != NULL && strcmp(cur_item->name, name) < 0) {
        prev_item = cur_item;
        cur_item = cur_item->next;
    }

    // insert "new_item" in between "prev_item" and "cur_item"
    new_item->next = cur_item;

    if (prev_item == NULL)
        t->arr_ptr[symtable_index] = new_item;
    else
        prev_item->next = new_item;
}


// Resizes array of symbol table "t" to "new_size". If allocation fails hash table remains unchanged.
void symtable_resize(symtable_t* t, const size_t new_size) {
    // cant resize to length 0
    if(new_size == 0) return;

    // allocate new arr_ptr array
    symtable_item_t** new_arr = malloc(new_size * sizeof(symtable_item_t*));
    if (new_arr == NULL) return;

    // initialize its elements to NULL
    for (size_t i = 0; i < new_size; ++i) new_arr[i] = NULL;

    // store old arr_size and old arr_ptr
    size_t old_size = t->arr_size;
    symtable_item_t** old_arr = t->arr_ptr;

    // update arr_size and arr_ptr to new values
    t->arr_size = new_size;
    t->arr_ptr = new_arr;

    // cur item and next item to be inserted to new arr_ptr
    symtable_item_t* cur_item, * next_item;

    // iterate through all old_arr items and add them to new_arr
    for (size_t i = 0; i < old_size; ++i) {
        // iterate trough item list at index i
        cur_item = old_arr[i];
        while (cur_item != NULL) {
            // add cur item to hash table
            next_item = cur_item->next;
            _symtable_resize_add(t, cur_item);
            cur_item = next_item;
        }
    }

    free(old_arr);
}

//============================

// Removes all items from symbol table.
void symtable_remove_all_items(symtable_t * t){
    // check for NULL pointer and for zero size
    if(t == NULL || t->symbol_count == 0) return;
    
    // pointers to current item and temporary item
    symtable_item_t *cur_item, *tmp_item;

    for(size_t i = 0; i < t->arr_size; ++i){

        // iterate trough list
        cur_item = t->arr_ptr[i];

        while(cur_item != NULL){
            // store next item and free cur item
            tmp_item = cur_item;
            cur_item = cur_item->next;
            symtable_item_free(tmp_item);
        }

        // set pointer to first element to NULL
        t->arr_ptr[i] = NULL;
    }

    t->symbol_count = 0; // update size
}


// Destructor: frees all symbol table "t" items and symbol table structure itself.
void symtable_free(symtable_t *t){
    if(t != NULL){
        symtable_remove_all_items(t);

        free(t->arr_ptr);

        free(t);
    }
}

// Removes all items from symbol table and resizes symbol table
void symtable_clear(symtable_t * t){
    if(t != NULL){
        symtable_remove_all_items(t);

        symtable_resize(t, INITIAL_SIZE);
    }
}

//============================

// Searches symbol table "t" for symbol with specified "name".
// If found, function returns pointer to corresponding symbol, otherwise NULL is returned.   
symbol_info_t * symtable_lookup(symtable_t * t, symbol_name_t name){
	// check for NULL pointers
	if(t == NULL || name == NULL)
		return NULL;

	// calculate symtable index
	const size_t symtable_index = hash_function(name) % t->arr_size;

	// get item at symtable_index
	symtable_item_t *cur_item = t->arr_ptr[symtable_index];

	// iterate trough item list until searched name is found
    int cmp_ret = 1;
	while(cur_item != NULL && (cmp_ret = strcmp(cur_item->name, name)) < 0){
		cur_item = cur_item->next;
	}

	// if found item return address of item symbol, otherwise return NULL
	if(cmp_ret == 0)
		return &(cur_item->symbol_info);
	else
		return NULL;
}

//============================

// Searches symbol table "t" for symbol with specified "name".
// If found, function returns pointer to corresponding symbol and sets "name_found" to true.
// If symbol was not found, new symbol with specified "name" is created, initialized and added to symbol table "t". 
// Afterwards pointer to newly added symbol is returned and "name_found" is set to false.
// If average length of symtable "t" exceeds AVG_LEN_MAX, function tries to resize symbol table array to double its size.
symbol_info_t * symtable_lookup_insert(symtable_t * t, symbol_name_t name, bool *name_found){
	// check for NULL pointers
	if(t == NULL || name == NULL) return NULL;

	// calculate symtable index
	const size_t symtable_index = hash_function(name) % t->arr_size;

	// get item at symtable_index
	symtable_item_t *cur_item = t->arr_ptr[symtable_index];
	symtable_item_t *prev_item = NULL;

	// iterate trough list until name is found or until list end is found
    int cmp_ret = 1;
	while(cur_item != NULL && (cmp_ret = strcmp(cur_item->name, name)) < 0){
		prev_item = cur_item;
		cur_item = cur_item->next;
	}
	
	if(cmp_ret == 0){// if symbol with searched name was found
        if(name_found != NULL) *name_found = true;
        
        // return already existing symbol
        return &(cur_item->symbol_info);
    }
    else{
        if(name_found != NULL) *name_found = false;

        // create new item
        symtable_item_t * new_item = symtable_item_create(name);
        if(new_item == NULL) return NULL;

        new_item->next = cur_item;

        // insert new item to list
        if(prev_item == NULL) // if list is empty
            t->arr_ptr[symtable_index] = new_item;
        else
            prev_item->next = new_item;
        
        // increment size
        ++(t->symbol_count);

        // if average items of symtable array lists is more then AVG_LEN_MAX, resize symtable
        if(t->symbol_count / t->arr_size > AVG_LEN_MAX)
            symtable_resize(t, (t->arr_size*2));

        // return newly added symbol
        return &(new_item->symbol_info);
    }
}








/*

#define MINIMUM_ARR_SIZE 32

// Minimum average length (length = symtable_size / symtable_bucket_count ) of symtable lists.
#define AVG_LEN_MIN 1

// Finds and removes record with specified "name" from hash table "t".
// If successfull, function returns true, otherwise false is returned.
// If average length of symtable "t" falls under AVG_LEN_MIN, function tries to resize table array to half its size.
bool symtable_erase(symtable_t * t, symbol_name_t name){
    // check for NULL pointers
	if(t == NULL || name == NULL) return false;

	// calculate symtable index
	const size_t symtable_index = symtable_hash_function(name) % t->arr_size;

	// get item at symtable_index
	symtable_item_t *cur_item = t->arr_ptr[symtable_index];
	symtable_item_t *prev_item = NULL;

	// variable used to hold strcmp result ititialized to non zero value
	int cmp_ret = 1;

	// iterate trough item list until searched name is found
	while(cur_item != NULL && (cmp_ret = strcmp(cur_item->name, name)) < 0){
		prev_item = cur_item;
		cur_item = cur_item->next;
	}

	if(cmp_ret == 0){ // if matching name found, then remove element
		// unlink cur_item from list
		if(prev_item == NULL)
			t->arr_ptr[symtable_index] = cur_item->next;
		else
			prev_item->next = cur_item->next;
		
		// free cur_item
		symtable_item_free(cur_item);

		// decrement size
		--(t->symbol_count);

		// if average items of symtable array lists is less then AVG_LEN_MIN, resize symtable to half the size
		if(t->symbol_count / t->arr_size < AVG_LEN_MIN && t->arr_size != MINIMUM_ARR_SIZE)
			symtable_resize(t, (t->arr_size/2));

		// return success
		return true;
	}
	else{
		return false;
	}
}

// Iterates trough all hash table "t" records and calls function "f" over each record.
void symtable_for_each(const symtable_t * t, void (*f)(symbol_t *symbol)){
    // check for NULL pointers
	if(t == NULL || f == NULL) return;

    // pointer to current item
	symtable_item_t *cur_item = NULL;

    // itereate trough all hash table items
    for(size_t index = 0; index < t->arr_size; ++index){
        // iterate trough item list
        for(cur_item = t->arr_ptr[index]; cur_item != NULL; cur_item = cur_item->next){
            f(&(cur_item->symbol));
        }
    }
}

// Returns size of hash table array of symtable "t".
size_t symtable_bucket_count(const symtable_t * t){
    return ((t) ? t->arr_size : 0);
}

// Returns number of items in hash table "t".
size_t symtable_size(const symtable_t * t){
    return ((t) ? t->symbol_count : 0);
}

*/