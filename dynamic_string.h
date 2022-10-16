/****
 ** dynamic_string.h
 ** Řešení IFJ-PROJEKT, 5.10.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file dynamic_string.h
 * @author xhorva17
 * @brief Dynamic string header file.
 * @date 2022-10-05
 */

#ifndef __DYNAMIC_STRING_H__
#define __DYNAMIC_STRING_H__

#include <stdlib.h>

typedef struct {
	char * str; // String.
	size_t size; // String length.
	unsigned wi; // Next index for write.
} dynamic_string_t;

/**
 * @brief Initialize a dynamic string.
 * @returns Initialized dynamic string. NULL if error.
 */
dynamic_string_t * ds_init();

/**
 * @brief Resize dynamic string to twice the current size.
 * @param ds Dynamic string to resize.
 * @returns 0 when successful, non-zero on error.
 */
unsigned ds_resize(dynamic_string_t * ds);

/**
 * @brief Deallocates dynamic string.
 * @param ds Dynamic string to deallocate.
 */
void ds_dstr(dynamic_string_t * ds);

/**
 * @brief Writes character at end of dynamic string. On error, string is deallocated.
 * @param ds Dynamic string for writing.
 * @param c Character to write.
 * @returns 0 when successful, non-zero on error.
 */
unsigned ds_write(dynamic_string_t * ds, int c);

#endif // __DYNAMIC_STRING_H__
