/****
 ** scanner.h
 ** Řešení IFJ-PROJEKT, 01.10.2022
 ** Autor: 
 ** Přeloženo: 
 **/

/**
 * @file children.h
 * @author
 * @brief Scanner header file.
 * @date 2022-10-01
 */


/* 
TOKENS:

    Attributed TOKENS:
        IDENTIFIER - identifier, attribute: {myfunc}
        VAR - variable, attribute: {$myvar}
        STR_LIT - string literal, attribute: VALUE {"text"}
        INT_LIT - integer numeric value, attribute: VALUE {7}
        FLT_LIT - real numeric value, attribute: VALUE {0.8}
        REL - relation token, attribute: {gt, lt, gte, lte, eq, neq}
        

    Un-Attributed TOKENS:
        Arithmetic:
            ADD - add: +
            SUB - minus: -
            MUL - mul: *
            DIV - div: /
    
        Punctuation:
            COMMA - comma: ,
            COLON - colon: :
            SCOLON - semicolon: ;
            LB - left bracket: (
            RB - right bracket: )
            LCB - left curly bracket: {
            RCB - right curly bracket: }
            ASSIGN - assign: =
        
        TODO: Proper keyword tokens.
        Keywords: Due to C reserved keywords, shortened versions where chosen.
            	ELSE, FUNCTION, IF, RETURN, WHILE
    
        Type keywords: Due to C reserved keywords, shortened versions where chosen.
		float, ?float, string, ?string, int, ?int,   null,   void
		^^
		FLT_T, NFLT_T, STR_T, NSTR_T, INT_T, NINT_T, NULL_T, VOID
*/

#include "dynamic_string.h"

#ifndef __SCANNER_H__
#define __SCANNER_H__

// A-Z, a-z
#define letter 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72: case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88: case 89: case 90: case 97: case 98: case 99: case 100: case 101: case 102: case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 110: case 111: case 112: case 113: case 114: case 115: case 116: case 117: case 118: case 119: case 120: case 121: case 122:

// 0-9
#define digit 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:

typedef enum {IDENTIFIER,VAR,STR_LIT,INT_LIT,FLT_LIT,REL,ADD,SUB,MUL,DIV,COMMA,COLON,SCOLON,LB,RB,LCB,RCB,ASSIGN,ELSE,FUNCTION,IF,RETURN,WHILE,FLT_T,NFLT_T,STR_T,NSTR_T,INT_T,NINT_T,NULL_T,VOID} token_type_t;

typedef enum {GT, LT, GTE, LTE, EQ, NEQ} rel_type_t;

typedef struct {
    const token_type_t type;
    union {
        char * sval;
        int ival;
        float fval;
        const rel_type_t rel_type;
    };
} token_t;

/**
 * @brief Scanner generates a token upon request from parser.
 * @returns Pointer to allocated token_t struct, NULL on failure.
 */
token_t * get_token();

/**
 * @brief Handles initialization of a keyword or identifer lexeme.
 * @returns
 */
void vik_handler(dynamic_string_t * ds, int * c);


/**
 * @brief Handles initialization of a keyword or identifer lexeme.
 * @returns
 */
void s_handler(dynamic_string_t * ds, int * c);

#endif // __SCANNER_H__
