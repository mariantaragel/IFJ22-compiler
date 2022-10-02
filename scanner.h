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
        i - identifier, attribute: {myfunc}
        v - variable, attribute: {$myvar}
        s - string literal, attribute: VALUE {"text"}
        z - integer numeric value, attribute: VALUE {7}
        r - real numeric value, attribute: VALUE {0.8}
        rel - relation token, attribute: {gt, lt, gte, lte, eq, neq}
        

    Un-Attributed TOKENS:
    
        Arithmetic:
            add - add: +
            sub - minus: -
            mul - mul: *
            dv - div: /
    
        Punctuation:
            com - comma: ,
            col - colon: :
            scol - semicolon: ;
            lb - left bracket: (
            rb - right bracket: )
            lcb - left curly bracket: {
            rcb - right curly bracket: }
            ass - assign: =
        
        TODO: Proper keyword tokens.
        Keywords: Due to C reserved keywords, shortened versions where chosen.
            els, func, iffy, ret, whil
            ^
            else, function, if, return, while
    
        Type keywords: Due to C reserved keywords, shortened versions where chosen.
            flt, nflt, str, nstr, it, nit, nl, vd
            ^
            flaot, ?float, string, ?string, int, ?int, null, void

*/

#ifndef __SCANNER_H__
#define __SCANNER_H__

// UGLY macro definitions for catching characters in case statements.

// A-Z, a-z
#define letter 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72: case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88: case 89: case 90: case 97: case 98: case 99: case 100: case 101: case 102: case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 110: case 111: case 112: case 113: case 114: case 115: case 116: case 117: case 118: case 119: case 120: case 121: case 122:

// 0-9
#define digit 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:

typedef enum {i,v,s,z,r,rel,add,sub,mul,dv,com,col,scol,lb,rb,lcb,rcb,ass,els,func,iffy,ret,whil,flt,nflt,str,nstr,it,nit,nl,vd} token_type_t;

typedef enum {gt, lt, gte, lte, eq, neq} rel_type_t;

typedef struct {
    const token_type_t type;
    union {
        char * sval;
        int ival;
        float fval;
        const rel_type_t rel_type;
    };
} token_t;

typedef struct {
   	char * lex; // Token string.
    	size_t size; // Size of lex.
	unsigned wi; // Write index.
} lexeme_t;

/**
 * @brief Scanner generates a token upon request from parser.
 * @returns Pointer to allocated token_t struct, NULL on failure.
 */
token_t * get_token(FILE * fp);

/**
 * @brief Initialization of a lexeme upon new token request. 
 * @returns lexeme_t struct. lexeme_t.lex requires check for NULL pointer, which is returned on failure.
 */
lexeme_t lex_init();

/**
 * @brief Deinitialization of a lexeme after token generation.
 */
void lex_dstr(lexeme_t lexeme);

/**
 * @brief Resizing of a lexeme.
 * @returns Resized lexeme, requires a check for lexeme_t.size.
 */
lexeme_t lex_resize(lexeme_t lexeme);

/**
 * @brief Handles initialization of a keyword or identifer lexeme.
 * @returns
 */
void vik_handler(int * c, FILE * fp, lexeme_t * lexeme);


/**
 * @brief Handles initialization of a keyword or identifer lexeme.
 * @returns
 */
void s_handler(int * c, FILE * fp, lexeme_t * lexeme);

#endif // __SCANNER_H__
