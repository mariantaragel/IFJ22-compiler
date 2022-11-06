/****
 ** scanner.c
 ** Řešení IFJ-PROJEKT, 01.10.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file scanner.c
 * @author xhorva17
 * @brief Scanner function implementations.
 * @date 2022-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include "scanner.h"
#include "dynamic_string.h"
#include "token.h"
#include "error.h"

/*
int main() {
	token_t * token;
	do {
		token = get_token();
		t_print(token);
		if(error == INTERNAL_ERROR) {
			fprintf(stderr, "INTERNAL_ERROR\n");
			return error;
		} else if (error == LEXICAL_ERROR) {
			fprintf(stderr, "LEXICAL_ERROR\n");
			return error;
		}
	} while(token->type != END && token->type != EPILOG);
	return 0;
}
*/

token_t * get_token() {
	dynamic_string_t * ds = ds_init(); // Initialize write buffer.
	if(ds == NULL) {
		error = INTERNAL_ERROR;
		return NULL;
	}
	token_t * t = t_init(); // Initialize returned token.
	if(t == NULL) {
		error = INTERNAL_ERROR;
		return NULL;
	}

	int c = 0; // Read character.
	do {
	/* Skip leading whitespace. */
		c = fgetc(stdin);
		/* Skip comment. */
		if(c == '/') {
			c = fgetc(stdin);
			if(c == '/') {
				do { 
					continue;
				} while( (c = fgetc(stdin)) != EOF && c != '\n');
			} else if (c == '*') {
				bool in_comment =  true;
				while(in_comment && c != EOF) {
					c = fgetc(stdin);
					if(c == '*') {
						if((c = fgetc(stdin)) == '/') {
							in_comment = false;
							c = fgetc(stdin);
						}
					}
				}
			} else {
				ungetc(c, stdin); // Return to stream due to false read..
				c = '/'; // Set char to original.
			}
		} // End comment skip.
	} while ( c != EOF && isspace(c) != 0);

	if(c == EOF) {
		ds_dstr(ds);
		t->type = END;
		return t;
	}
	
	switch(c) {
		/* ATTRIBUTED TOKENS, KEYWORDS AND TYPES*/
		case letter case '_': vik_handler(ds, t, &c); break; // FUNC_ID, FLT_T, STR_T, INT_T, VOID_T, NULL_LIT, ELSE, FUNCTION, IF, RETURN, WHILE
		case '$': vik_handler(ds, t, &c); break; // VAR_ID
		case digit fi_handler(ds, t, &c); break; // INT_LIT, FLT_LIT
		case '"': s_handler(ds, t, &c); break; // STR_LIT
		case '?': nte_handler(ds, t, &c); break; // NFLT_T, NINT_T, NSTR_T, EPILOG
		/* UNATTRIBUTED TOKENS */
		/* Operators */
		case '*': t->type = MUL; break; // MUL
		case '/': t->type = DIV; break; // DIV
		case '+': t->type = ADD; break; // ADD
		case '-': t->type = SUB; break; // SUB
		case '.': t->type = CONCAT; break; // CONCAT
		case '<': lp_handler(ds, t, &c); break; // LT, LTE, PROLOG
		case '>': g_handler(t, &c); break; // GT, GTE
		case '!': neq_handler(t, &c); break; // NEQ
		case '=': aeq_handler(t, &c); break; // ASSIGN, EQ
		/* Punctuation */
		case ',': t->type = COMMA; break; // COMMA
		case ':': t->type = COLON; break; // COLON
		case ';': t->type = SCOLON; break; // SCOLON
		case '(': t->type = LB; break; // LB
		case ')': t->type = RB; break; // RB
		case '{': t->type = LCB; break; // LCB
		case '}': t->type = RCB; break; // RCB
		default: error = LEXICAL_ERROR; break; // Couldn't match, throw error.
	}
	ds_dstr(ds);
	return t;
}

void vik_handler(dynamic_string_t * ds, token_t * t, int * c) {
	static const char * reserved_words[] = {"else", "function", "if", "return", "while", "float", "string", "int", "null", "void"};
	static const int reserved_words_count = 10;
	do {
		if(ds_write(ds, *c)) { // Write character.
			error = INTERNAL_ERROR;
			return;
		}
	} while( (*c = fgetc(stdin)) != EOF && (isalnum(*c) != 0 || *c == '_') && isspace(*c) == 0); // watchout for file.
	ungetc(*c, stdin);

	for(int i = 0; i < reserved_words_count; i++) {
		/* Try matching with reserved keywords */
		if(strcmp(reserved_words[i], ds->str) == 0) {
			switch(i) {
				case 0: t->type = ELSE; break;
				case 1: t->type = FUNCTION; break;
				case 2: t->type = IF; break;
				case 3: t->type = RETURN; break;
				case 4: t->type = WHILE; break;
				case 5: t->type = FLT_T; break;
				case 6: t->type = STR_T; break;
				case 7: t->type = INT_T; break;
				case 8: t->type = NULL_LIT; break;
				case 9: t->type = VOID_T; break;
				default: break;
			}
			return; // Match found, return.
		}
	}
	/* No match found, token is func/variable identifier. */
	if(ds->str[0] == '$') {
		// TODO: str[1] cannot be NUMBER!!!
		t->type = VAR_ID;
	} else {
		t->type = FUNC_ID;
	}
	/* Attach attribute */
	t->sval = malloc(strlen(ds->str) + 1);
	if(t->sval == NULL) {
		error = INTERNAL_ERROR;
	}
	strcpy(t->sval, ds->str);
}

void fi_handler(dynamic_string_t * ds, token_t * t, int * c) {
	do {
		if(ds_write(ds, *c)) {
			error = INTERNAL_ERROR;
			return;
		}
	} while( (*c = fgetc(stdin)) != EOF && isdigit(*c));

	if(*c != '.') {
		/* Check if value is correct */
		ungetc(*c, stdin);
		t->ival = strtoimax(ds->str, NULL, 10); // If valid conversion, store ds as string...
		t->type = INT_LIT;
	} else {
		do {
			if(ds_write(ds, *c)) {
				error = INTERNAL_ERROR;
				return;
			}
		} while( (*c = fgetc(stdin)) != EOF && isdigit(*c));
		ungetc(*c, stdin);
		t->fval = strtod(ds->str, NULL);
		t->type = FLT_LIT;
	}
}

/* TODO: Finish interpolation. */
void s_handler(dynamic_string_t * ds, token_t * t, int * c) {
	
	while( (*c = fgetc(stdin)) != EOF && *c != '"') { // Write string to buffer.

		if(*c == '\\') { // Escape sequence.
			dynamic_string_t * aux_ds = ds_init();
			if(aux_ds == NULL) {
				error = INTERNAL_ERROR;
				return;
			}
			*c = fgetc(stdin);
			switch(*c) {
				case 'n': if(ds_write(ds, '\n')) { error = INTERNAL_ERROR; return; } break;
				case 't': if(ds_write(ds, '\t')) { error = INTERNAL_ERROR; return; } break;
				case '"': if(ds_write(ds, '"')) { error = INTERNAL_ERROR; return; } break;
				case '\\': if(ds_write(ds, '\\')) { error = INTERNAL_ERROR; return; } break;
				case '$': if(ds_write(ds, '$')) { error = INTERNAL_ERROR; return; } break;
				case '0': // Octal conversion of three characters.
				case 'x': // Hexadecimal conversion of two characters.
				default: 
					if(ds_write(ds, '\\')) { error = INTERNAL_ERROR; return; }
					if(ds_write(ds,*c)) { error = INTERNAL_ERROR; return; }
					break;
			}
		} else {
			if(ds_write(ds, *c)) {
				error = INTERNAL_ERROR;
				return;
			}
		}
	} // End write.

	/* Set type and save associated value. */
	t->type = STR_LIT;
	t->sval = malloc(strlen(ds->str) + 1);
	if(t->sval == NULL) {
		error = INTERNAL_ERROR;
	}
	strcpy(t->sval, ds->str);
}

void nte_handler(dynamic_string_t * ds, token_t * t, int * c) {
	static const char * reserved_words[] = {"?float", "?string", "?int"};
	static const int reserved_words_count = 3;

	*c = fgetc(stdin);
	if( *c == '>') {
		t->type = EPILOG; // '?>'
		return;
	} else { // Return character to stream if unsuccesful.
		ungetc(*c, stdin);
		*c = '?'; // Set to original.
	}


	do { // Read all letters.
		if(ds_write(ds, *c)) {
			error = INTERNAL_ERROR;
			return;
		}
	} while( (*c = fgetc(stdin)) != EOF && isalpha(*c) != 0);
	ungetc(*c, stdin);
	

	/* Try matching with reserved keywords */
	for(int i = 0; i < reserved_words_count; i++) {
		if(strcmp(reserved_words[i], ds->str) == 0) {
			switch(i) {
				case 0: t->type = NFLT_T; break; // '?float'
				case 1: t->type = NSTR_T; break; // '?string'
				case 2: t->type = NINT_T; break; // '?int'
				default: break;
			}
			return; // Match found. 
		}
	}
	error = LEXICAL_ERROR; // Couldn't match any reserved word.
}

void lp_handler(dynamic_string_t * ds, token_t * t, int * c) {
	static const char * prolog = "php";
	*c = fgetc(stdin);
	if(*c == '=') { // '<='
		t->type = LTE;
	} else if (*c == '?') { // '<?' try create '<?php'
		while((*c = fgetc(stdin)) != EOF && isspace(*c) == 0) {
			if(ds_write(ds, *c)) {
				error = INTERNAL_ERROR;
				return;
			}
		}
		if(strcmp(ds->str, prolog) == 0) { // '<?php'
			t->type = PROLOG;
			return;
		} else {
			error = LEXICAL_ERROR;
		}
	} else { // '<'
		t->type = LT;
		ungetc(*c, stdin);
	}
}

void g_handler(token_t * t, int * c) {
	*c = fgetc(stdin);
	if(*c == '=') {
		t->type = GTE;
	} else {
		t->type = GT;
		ungetc(*c, stdin);
	}
}

void neq_handler(token_t * t, int * c) {
	*c = fgetc(stdin);
	if(*c == '=') { // '!='
		*c = fgetc(stdin);
		if(*c == '=') { // '!=='
			t->type = NEQ;
		} else {
			error = LEXICAL_ERROR;
		}
	} else {
		error = LEXICAL_ERROR;
	}
}

void aeq_handler(token_t * t, int * c) {
	*c = fgetc(stdin);
	if(*c == '=') { // Try to create '==='
		*c = fgetc(stdin);
		if(*c == '=') {
			t->type = EQ;
		} else { // Only '=='
			error = LEXICAL_ERROR;
		}
	} else {
		t->type = ASSIGN;
		ungetc(*c, stdin); // Return extra read.
	}
}

