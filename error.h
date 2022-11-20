/****
 ** error.h
 ** Řešení IFJ-PROJEKT, 20.10.2022
 ** Autor: xhorva17 xtarag01
 ** Přeloženo:
 **/

/**
 * @file error.h
 * @author xhorva17 xtarag01
 * @brief Error handling header file.
 * @date 2022-10-20
 */


#ifndef ERROR_H
#define ERROR_H

typedef enum error {OK, LEXICAL_ERROR, SYNTAX_ERROR, SEM_ERROR_3, SEM_ERROR_4, SEM_ERROR_5, SEM_ERROR_6, SEM_ERROR_7, SEM_ERROR_8, INTERNAL_ERROR = 99} error_codes_t;

// Global error variable.
extern error_codes_t error;

/**
 * @brief Prints error status along with error code.
 * @param error Global error variable.
*/
void error_print(error_codes_t error);

#endif // ERROR_H
