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
#include <math.h>
#include <stdbool.h>
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
	if(str == NULL) {
		return ds_init(); // Return empty dynamic_string if string is NULL.
	}
	dynamic_string_t * ds = (dynamic_string_t*) calloc(1, sizeof(dynamic_string_t)); // Allocate ds struct
	if(ds == NULL) {
		return NULL;
	}
	size_t size = DS_SIZE;
	while(size <= strlen(str)) {
		size *= 2;
	}
	ds->str = (char*) malloc(size*sizeof(char)); // Allocate string buffer with write reserve.

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
	if(ds == NULL) {
		return;
	}
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


// can be done as ds_concat_str wher
int ds_concat(dynamic_string_t * a, dynamic_string_t * b) {
	if(a == NULL || b == NULL) {
		return 1;
	}
	if(ds_concat_str(a, b->str)) {
		return 1;
	}
	return 0;
}

int ds_concat_str(dynamic_string_t * ds, const char * str) {
	if(str == NULL) {
		return 0; // Change nothing when NULL string.
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

/* USE ONLY POSITIVE VALUES GREATER THAN 1, other values are undefined.*/
int ds_write_uint(dynamic_string_t * ds, unsigned n) {
	if(n == 0) { // Look into this...
		return 1;
	}
	size_t dummy_size =  (int)((ceil(log10(n))+2)*sizeof(char));
	char dummy[dummy_size];
	sprintf(dummy, "%d", n);
	if(ds_concat_str(ds, dummy)) {
		return 1;
	}
	return 0;
}

char * ds_get_str(dynamic_string_t * ds, bool free_ds) {
	char * str = malloc(strlen(ds->str) + 1);
	if(str == NULL) {
		return NULL;
	}
	strcpy(str, ds->str);
	if(free_ds) {
		ds_dstr(ds);
	}
	return str;
}
