/****
 ** token.h
 ** Řešení IFJ-PROJEKT, 7.10.2022
 ** Autor: 
 ** Přeloženo: 
 **/

/**
 * @file token.h
 * @author
 * @brief Token header file.
 * @date 2022-10-07
 */



/* 
TOKENS:

    Attributed TOKENS:
        FUNC_ID - identifier, attribute: {myfunc}
        VAR_ID - variable, attribute: {$myvar}
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
		FLT_T, NFLT_T, STR_T, NSTR_T, INT_T, NINT_T, NULL_T, VOID_T
*/

#ifndef __TOKEN_H__
#define __TOKEN_H__

typedef enum {FUNC_ID,VAR_ID,STR_LIT,INT_LIT,FLT_LIT,REL,ADD,SUB,MUL,DIV,COMMA,COLON,SCOLON,LB,RB,LCB,RCB,ASSIGN,ELSE,FUNCTION,IF,RETURN,WHILE,FLT_T,NFLT_T,STR_T,NSTR_T,INT_T,NINT_T,NULL_T,VOID_T} token_type_t;

typedef struct {
	token_type_t type;
	union {
		char * sval;
		int ival;
		float fval;
	};
} token_t;

/**
 * @brief Initialize a token.
 * @returns Initialized token. NULL if error.
 */
token_t * t_init();

/**
 * @brief Deallocates token.
 * @param t Token to deallocate.
 */
void t_dstr(token_t * t);


/**
 * @brief Prints token attributes for logging.
 * @param t Token to print.
 */
void t_print(token_t * t);


#endif // __TOKEN_H__
