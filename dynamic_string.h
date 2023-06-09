/**
 * @file dynamic_string.h
 * @author Martin Horvat, xhorva17
 * @brief Header file for dynamic string.
 * @date 2022-10-05
 */

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	char * str; // String to write into.
	size_t size; // Size of currently allocated string buffer.
	unsigned wi; // Next index of writing into str.
} dynamic_string_t;

/**
 * @brief Initialize an empty dynamic string.
 * @returns Initialized dynamic string. NULL if error.
 */
dynamic_string_t * ds_init();

/**
 * @brief Initialize dynamic string with a given string.
 * @param str String with which the ds is initialized.
 * @returns Initialized dynamic string. NULL if error.
*/
dynamic_string_t * ds_strinit(const char * str);

/**
 * @brief Resize dynamic string to twice the current size.
 * @param ds Dynamic string to resize.
 * @returns 0 when successful, non-zero on error.
 */
int ds_resize(dynamic_string_t * ds);

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
int ds_write(dynamic_string_t * ds, int c);

/**
 * @brief Concatenates the dynamic string from a onto b. Operation: a.b
 * @param a Dynamic string which will be concatenated.
 * @param b Dyanmic string which is concatenated onto a.
 */
int ds_concat(dynamic_string_t * a, dynamic_string_t * b);

/**
 * @brief Concatenates a string literal onto a dynamic string.
 * @param ds Dynamic string which will be concatenated.
 * @param str String literal which is concatenated onto ds.
*/
int ds_concat_str(dynamic_string_t * ds, const char * str);

/**
 * @brief Writes an unsigned integer as a string value onto the end of a dynamic string.
 * @param ds Dynamic string for writing.
 * @param n Integer to write.
*/
int ds_write_uint(dynamic_string_t * ds, unsigned n);

/**
 * @brief Returns the string contained in ds.
 * @param ds Dynamic string with string.
 * @param free_ds Deinitialize ds if true.
 * @returns String from ds, NULL on failure.
*/
char * ds_get_str(dynamic_string_t * ds, bool free_ds);

#endif // DYNAMIC_STRING_H
