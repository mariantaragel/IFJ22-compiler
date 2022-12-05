/****
 ** token.h
 ** Řešení IFJ-PROJEKT, 7.10.2022
 ** Autor: xhorva17
 ** Přeloženo: 
 **/

/**
 * @file token.h
 * @author
 * @brief Token header file.
 * @date 2022-10-07
 */



/* 
TOKEN TABLE:
    Attributed TOKENS:
	function_identifier -> FUNC_ID : {string(function_identifier}
	$variable_identifier -> VAR_ID : {string(variable_identifier)}
	"A string literal" -> STR_LIT : {string(string_literal)}
	integer_literal -> INT_LIT : {int(value)}
	float_literal -> FLT_LIT : {float(value)}

    Unattributed TOKENS:
	Operators:
		* -> MUL
		/ -> DIV
		+ -> ADD
		- -> SUB
		. -> CONCAT
		< -> LT
		> -> GT
		<= -> LTE
		>= -> GTE
		=== -> EQ
		!== -> NEQ
        Punctuation:
		, -> COMMA
		: -> COLON
		; -> SCOLON
		( -> LB
		) -> RB
		{ -> LCB
		} -> RCB
		= -> ASSIGN
        Types:
		float -> FLT_T
		string -> STR_T
		int -> INT_T
		void -> VOID_T
		null -> NULL_LIT
		?float -> NFLT_T
		?int -> NINT_T
		?string -> NSTR_T
	Keywords:
		else -> ELSE
		function -> FUNCTION
		if -> IF
		return -> RETURN
		while -> WHILE	
	Other:
		<?php -> PROLOG
		?> -> EPILOG
		EOF -> END
		?? -> NONE
		declare(strict_types=1); -> ??
		
*/

#ifndef TOKEN_H
#define TOKEN_H

typedef enum {FUNC_ID,VAR_ID,STR_LIT,INT_LIT,FLT_LIT,MUL,DIV,ADD,SUB,CONCAT,LT,GT,LTE,GTE,EQ,NEQ,COMMA,COLON,SCOLON,LB,RB,LCB,RCB,ASSIGN,FLT_T,STR_T,INT_T,VOID_T,NULL_LIT,NFLT_T,NINT_T,NSTR_T,ELSE,FUNCTION,IF,RETURN,WHILE,PROLOG,EPILOG,END} token_type_t;

typedef struct {
	token_type_t type;
	char * aval; // Associated value.
} token_t;

/**
 * @brief Initialize a token.
 * @returns Initialized token. NULL if error.
 */
token_t * t_init();

/**
 * @brief Duplicates a token.
 * 
 * @param t Token to duplicate.
 * @return token_t* 
 */
token_t * t_dup(const token_t * t);

/**
 * @brief Attaches an associated value to a token.
 * 
 * @param str 
 * @return int 
 */
int t_attach(token_t * t, const char * str);

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

#endif // TOKEN_H
