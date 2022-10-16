/****
 ** scanner.h
 ** Řešení IFJ-PROJEKT, 01.10.2022
 ** Autor: xhorva17
 ** Přeloženo: 
 **/

/**
 * @file children.h
 * @author xhorva17
 * @brief Scanner header file.
 * @date 2022-10-01
 */

#include "dynamic_string.h"
#include "token.h"

#ifndef __SCANNER_H__
#define __SCANNER_H__

// A-Z, a-z
#define letter 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72: case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88: case 89: case 90: case 97: case 98: case 99: case 100: case 101: case 102: case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 110: case 111: case 112: case 113: case 114: case 115: case 116: case 117: case 118: case 119: case 120: case 121: case 122:

// 0-9
#define digit 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:

/**
 * @brief Scanner generates a token upon request from parser.
 * @returns Pointer to allocated token_t struct, NULL on failure.
 */
token_t * get_token();

/**
 * @brief Handles initialization of a keyword or identifer lexeme.
 * @returns
 */
void vik_handler(dynamic_string_t * ds, token_t * t, int * c);

/**
 * @brief Handles initialization of a keyword or identifer lexeme.
 * @returns
 */
void s_handler(dynamic_string_t * ds, token_t * t, int * c);


/**
 * @brief Handles...
 */
void fi_handler(dynamic_string_t * ds, token_t * t, int * c);

void single_char_token_handler(dynamic_string_t * ds, token_t * t, int * c);

void null_t_handler(dynamic_string_t * ds, token_t * t, int * c);

#endif // __SCANNER_H__
