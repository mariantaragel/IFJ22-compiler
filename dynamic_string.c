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
#include <string.h>
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

dynamic_string_t * ds_strinit(const char * str) {
	dynamic_string_t * ds = (dynamic_string_t*) calloc(1, sizeof(dynamic_string_t)); // Allocate ds struct
	if(ds == NULL) {
		return NULL;
	}
	size_t size = DS_SIZE;
	while(size <= strlen(str)) {
		size *= 2;
	}
	ds->str = (char*) malloc(size); // Allocate string buffer with write reserve.

	if(ds->str == NULL) {
		free(ds);
		return NULL;
	}

	strcpy(ds->str, str); // Attach to dynamic string...
	ds->size = size;
	ds->wi = strlen(str); // First index after end of string.

	return ds;
}

int ds_resize(dynamic_string_t * ds) {
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

int ds_write(dynamic_string_t * ds, int c) {
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

int ds_concat(dynamic_string_t * a, dynamic_string_t * b) {
	if(a->str == NULL || b->str == NULL) {
		return 1;
	} 
	// get length of string b... resize a to fit + write buffer...
	size_t new_size = a->size;
	while(new_size <= strlen(a->str) + strlen(b->str)) {
		new_size *= 2;
	}
	char * new_str = (char*) realloc(a->str, new_size); // Double size of string.
	if(new_str == NULL) {
		return 1;
	}
	strcat(new_str, b->str);

	a->str = new_str;
	a->size = new_size;
	a->wi = strlen(new_str); // Length of a + b => index of writing...
	return 0;
	
}

int ds_concat_str(dynamic_string_t * ds, const char * str) {
	if(ds->str == NULL || str == NULL) {
		return 1;
	}

	size_t new_size = ds->size;
	while(new_size <= strlen(ds->str) + strlen(str)) {
		new_size *= 2;
	}
	char * new_str = (char*) realloc(ds->str, new_size);
	if(new_str == NULL) {
		return 1;
	}
	strcat(new_str, str);

	ds->str = new_str;
	ds->size = new_size;
	ds->wi = strlen(new_str);
	return 0;
}

// int ds_write_int(dynamic_string_t * ds, int n) {
// 	ds->wi++;
// 	n++;
// 	return 1;
// }