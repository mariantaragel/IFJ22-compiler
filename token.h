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
		null -> NULL_T
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

#ifndef __TOKEN_H__
#define __TOKEN_H__

typedef enum {FUNC_ID,VAR_ID,STR_LIT,INT_LIT,FLT_LIT,MUL,DIV,ADD,SUB,CONCAT,LT,GT,LTE,GTE,EQ,NEQ,COMMA,COLON,SCOLON,LB,RB,LCB,RCB,ASSIGN,FLT_T,STR_T,INT_T,VOID_T,NULL_T,NFLT_T,NINT_T,NSTR_T,ELSE,FUNCTION,IF,RETURN,WHILE,PROLOG,EPILOG,END} token_type_t;

typedef struct {
	token_type_t type;
	union {
		char * sval;
		int ival;
		double fval;
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
