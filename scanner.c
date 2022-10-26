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

int main() {
	token_t * token;
	while( (token = get_token()) != NULL) {
		t_print(token);
		if(error != OK) {
			printf("\n\nERROR\n");
		}
	}
	if(token)
		return 1;
	return 0;
}

token_t * get_token() {
	dynamic_string_t * ds = ds_init();
	if(ds == NULL) {
		error = INTERNAL_ERROR;
		return NULL;
	}
	token_t * t = t_init();
	if(t == NULL) {
		error = INTERNAL_ERROR;
		return NULL;
	}
	
	int c = 0;
	
	do {
	/* Eat up leading whitespace. TODO: EOF checks if EOF found?? */
		c = fgetc(stdin);
		/* Eat up if comment... */
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
				ungetc(c, stdin); // Return false read.
				c = '/'; // Set char to original.
			}
		}
	} while ( c != EOF && isspace(c) != 0);

	if(c == EOF) {
		ds_dstr(ds);
		return NULL;
	}
	
	/* Skip comments. */

	switch(c) {
		/* ATTRIBUTED TOKENS, KEYWORDS AND TYPES*/
		case letter case '_': vik_handler(ds, t, &c); break; // FUNC_ID, FLT_T, STR_T, INT_T, VOID_T, NULL_T, ELSE, FUNCTION, IF, RETURN, WHILE
		case digit fi_handler(ds, t, &c); break; // INT_LIT, FLT_LIT
		case '$': vik_handler(ds, t, &c); break; // VAR_ID
		case '"': s_handler(ds, t, &c); break; // STR_LIT
		case '?': null_t_handler(ds, t, &c); break; // NFLT_T, NINT_T, NSTR_T, EPILOG
		
		/* UNATTRIBUTED TOKENS */
		/* Operators */
		case '*': t->type = MUL; break; // MUL
		case '/': t->type = DIV; break; // DIV
		case '+': t->type = ADD; break; // ADD
		case '-': t->type = SUB; break; // SUB
		case '.': t->type = CONCAT; break; // CONCAT
		case '<': lp_handler(t, &c); break; // TODO --> LT, LTE, PROLOG
		case '>': g_handler(t, &c); break; // TODO --> GT, GTE
		case '!': neq_handler(t, &c); break; // TODO --> NEQ
		case '=': aeq_handler(t, &c); break; // TODO --> ASSIGN, EQ
		/* Punctuation */
		case ',': t->type = COMMA; break; // COMMA
		case ':': t->type = COLON; break; // COLON
		case ';': t->type = SCOLON; break; // SCOLON
		case '(': t->type = LB; break; // LB
		case ')': t->type = RB; break; // RB
		case '{': t->type = LCB; break; // LCB
		case '}': t->type = RCB; break; // RCB
		default: error = LEXICAL_ERROR; break; // Throw LEXICAL error.
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
				case 8: t->type = NULL_T; break;
				case 9: t->type = VOID_T; break;
				default: break;
			}
			return; // ???
		}
		/* No keywords matched, set string as function identifier. */
		if(i == reserved_words_count - 1) {
			if(ds->str[0] == '$') {
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
	}
}

void fi_handler(dynamic_string_t * ds, token_t * t, int * c) {
	do {
		if(ds_write(ds, *c)) {
			error = INTERNAL_ERROR;
			return;
		}
	} while( (*c = fgetc(stdin)) != EOF && isdigit(*c));
	if(*c != '.') {
		ungetc(*c, stdin);
		t->ival = strtoimax(ds->str, NULL, 10); 
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
		//strtof
	}
}

void null_t_handler(dynamic_string_t * ds, token_t * t, int * c) {
	static const char * reserved_words[] = {"?float", "?string", "?int", "?>"};
	static const int reserved_words_count = 4;
	do {
		if(ds_write(ds, *c)) { // Write character.
			error = INTERNAL_ERROR;
			return;
		}
	} while( (*c = fgetc(stdin)) != EOF && isalpha(*c) != 0 && isspace(*c) == 0); // watchout for file.
	ungetc(*c, stdin);
	
	for(int i = 0; i < reserved_words_count; i++) {
		/* Try matching with reserved keywords */
		if(strcmp(reserved_words[i], ds->str) == 0) {
			switch(i) {
				case 0: t->type = NFLT_T; break;
				case 1: t->type = NSTR_T; break;
				case 2: t->type = NINT_T; break;
				default: break;
			}
			return; // ???
		}
		if(i == reserved_words_count - 1) {
			error = LEXICAL_ERROR;
		}
	}
}

void s_handler(dynamic_string_t * ds, token_t * t, int * c) {
	while( (*c = fgetc(stdin)) != EOF && *c != '"') {
		if(ds_write(ds, *c)) {
			error = INTERNAL_ERROR;
			return;
		}
	}
	t->type = STR_LIT;
	t->sval = malloc(strlen(ds->str) + 1);
	if(t->sval == NULL) {
		error = INTERNAL_ERROR;
	}
	strcpy(t->sval, ds->str);
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

void aeq_handler(token_t * t, int * c) {
	*c = fgetc(stdin);
	if(*c == '=') {
		*c = fgetc(stdin);
		if(*c == '=') {
			t->type = EQ;
		} else {
			fprintf(stderr, "Lexeme error.");
		}
	} else {
		t->type = ASSIGN;
		ungetc(*c, stdin);
	}
}

/* rewrite */
void neq_handler(token_t * t, int * c) {
	*c = fgetc(stdin);
	if(*c != '=') {
		error = LEXICAL_ERROR;
	} else {
		*c = fgetc(stdin);
		if(*c != '=') {
			error = LEXICAL_ERROR;
		} else {
			t->type = NEQ;
			return;
		}
	}
}

void lp_handler(token_t * t, int * c) {
	*c = fgetc(stdin);
	if(*c == '=') {
		t->type = LTE;
	} else {
		t->type = LT;
		ungetc(*c, stdin);
	}
	
}

