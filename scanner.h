/****
 ** scanner.h
 ** Řešení IFJ-PROJEKT, 01.10.2022
 ** Autor: Martin Horvat, xhorva17
 **/

/**
 * @file scanner.h
 * @author Martin Horvat, xhorva17
 * @brief Scanner header file.
 * @date 2022-10-01
 */

#include "dynamic_string.h"
#include "token.h"

#ifndef SCANNER_H
#define SCANNER_H

/**
 * @brief Reads a lexeme from standard input and tries to create a recognized token.
 * @returns Initialized token, NULL on failure.
 */
token_t * get_token();

#endif // SCANNER_H
