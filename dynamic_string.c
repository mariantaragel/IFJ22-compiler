/****
 ** dynamic_string.c
 ** Řešení IFJ-PROJEKT, 5.10.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file dynamic_string.h
 * @author xhorva17
 * @brief Dynamic string implementation.
 * @date 2022-10-05
 */

#include <stdlib.h>
#include <stdio.h>
#include "dynamic_string.h"

#define DS_SIZE 128

// TODO: Test resizing + free + memory leaks.

dynamic_string_t * ds_init() {
	dynamic_string_t * ds = (dynamic_string_t*) calloc(1, sizeof(dynamic_string_t)); // Allocate ds struct.
	if(ds == NULL) {
		return NULL;
	}
	char * str = (char*) calloc(1, DS_SIZE*sizeof(char)); // Allocate string.
	if(str == NULL) {
		free(ds);
		return NULL;
	}
	ds->str = str;
	ds->size = DS_SIZE;
	ds->wi = 0;
	return ds;
}

unsigned ds_resize(dynamic_string_t * ds) {
	char * new_str = (char*) realloc(ds->str, ds->size*2); // Double size of string.
	if(new_str == NULL) {
		// TODO: Handle realloc error.
		return 1;
	}
	ds->str = new_str;
	ds->size = ds->size*2;
	return 0;
}

void ds_dstr(dynamic_string_t * ds) {
	ds->size = 0;
	ds->wi = 0;
	free(ds->str);
	free(ds); // Free self.
}

unsigned ds_write(dynamic_string_t * ds, int c) {
	ds->str[ds->wi] = c;
	ds->wi++;
	if(ds->wi == ds->size) { // Resize if string limit reached.
		if(ds_resize(ds)) { // Handle realloc error.
			ds_dstr(ds); // De-init self if error occured.
			return 1;
		}
	}
	return 0;
}


