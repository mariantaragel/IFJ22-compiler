/****
 ** scanner.c
 ** Řešení IFJ-PROJEKT, 01.10.2022
 ** Autor: Martin Horvat, xhorva17
 **/

/**
 * @file scanner.c
 * @author Martin Horvat, xhorva17
 * @brief Implementation of lexical analyzer.
 * @date 2022-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include "limits.h"
#include "scanner.h"
#include "dynamic_string.h"
#include "token.h"
#include "error.h"

// A-Z, a-z
#define letter 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72: case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88: case 89: case 90: case 97: case 98: case 99: case 100: case 101: case 102: case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 110: case 111: case 112: case 113: case 114: case 115: case 116: case 117: case 118: case 119: case 120: case 121: case 122:

// 0-9
#define digit 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56: case 57:

/* Lexeme handlers */

void vik_handler(dynamic_string_t * ds, token_t * t, int * c);
void fi_handler(dynamic_string_t * ds, token_t * t, int * c);
void s_handler(dynamic_string_t * ds, token_t * t, int * c);
void nte_handler(dynamic_string_t * ds, token_t * t, int * c);
void lp_handler(dynamic_string_t * ds, token_t * t, int * c);
void g_handler(token_t * t, int * c);
void neq_handler(token_t * t, int * c);
void aeq_handler(token_t * t, int * c);
int oct_write(dynamic_string_t * ds, int * c);
int hex_write(dynamic_string_t * ds, int * c);
int float_write(dynamic_string_t * ds, int * c);


/* Testing main, Makefile -> make scanner */
// int main() {
//
// 	token_t * token;
// 	do {
// 		token = get_token();
// 		t_print(token);
		
// 		if(error == INTERNAL_ERROR) {
// 			fprintf(stderr, "INTERNAL_ERROR\n");
// 			return error;
// 		} else if (error == LEXICAL_ERROR) {
// 			fprintf(stderr, "LEXICAL_ERROR\n");
// 			return error;
// 		}
// 	} while(token->type != END && token->type != EPILOG);
// 	return 0;
// }


token_t * get_token() {
	static bool prolog_found = false; // Check for prolog as first token.
	dynamic_string_t * ds = ds_init(); // Initialize write buffer.
	if(ds == NULL) {
		error = INTERNAL_ERROR;
		return NULL;
	}
	token_t * t = t_init(); // Initialize token for return.
	if(t == NULL) {
		error = INTERNAL_ERROR;
		return NULL;
	}

	int c = 0; // Character for read/write.
	
	if(!prolog_found) { // Get prolog first.
		while((c = fgetc(stdin)) != EOF && !isspace(c)) {
			if(ds_write(ds, c)) {
				error = INTERNAL_ERROR;
				ds_dstr(ds);
				return t;
			}
		}
		if(strcmp(ds->str, "<?php") == 0) {
			prolog_found = true;
			t->type = PROLOG;
			ds_dstr(ds);
			return t;
		} else {
			error = LEXICAL_ERROR;
			ds_dstr(ds);
			return t;
		}
	}


	do { // Skip leading whitespace and comments.
		c = fgetc(stdin);
		if(c == '/') { // Skip if comment.
			c = fgetc(stdin);
			if(c == '/') {
				do { 
					continue;
				} while( (c = fgetc(stdin)) != EOF && c != '\n');
			} else if (c == '*') {
				bool in_comment = true;
				while(in_comment) {
					c = fgetc(stdin);
					if(c == EOF) { // Unterminated comment.
						ds_dstr(ds);
						error = LEXICAL_ERROR;
						return t;
					}
					if(c == '*') {
						c = fgetc(stdin);
						if(c == '/') {
							in_comment = false;
						} else if (c == EOF) { // Unterminated comment.
							ds_dstr(ds);
							error = LEXICAL_ERROR;
							return t;
						}
					}
				}
				c = ' '; // Force loop condition as true.
				continue;
			} else {
				ungetc(c, stdin); // Return to false read.
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
		/* ATTRIBUTED TOKENS, KEYWORDS AND TYPES */
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
	} while( (*c = fgetc(stdin)) != EOF && (isalnum(*c) != 0 || *c == '_') && !isspace(*c));
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
		if(strlen(ds->str) == 1) { // Invalid variable name length.
			error = LEXICAL_ERROR;
			return;
		} else if (isalpha(ds->str[1]) == 0 && ds->str[1] != '_') { // var identifer doesn't start with letter or '_'
			error = LEXICAL_ERROR;
			return;
		}
		t->type = VAR_ID;
	} else {
		t->type = FUNC_ID;
	}
	
	/* Attach attribute */
	if(t_attach(t, ds->str)) {
		error = INTERNAL_ERROR;
	};
}

void fi_handler(dynamic_string_t * ds, token_t * t, int * c) {
	do { // Write rest of digits.
		if(ds_write(ds, *c)) {
			error = INTERNAL_ERROR;
			return;
		}
	} while( (*c = fgetc(stdin)) != EOF && isdigit(*c));

	if(*c != '.' && *c != 'e' && *c != 'E') { // Value should be an integer literal.
		ungetc(*c, stdin); // Return false read.

		/* Check if value is ok. */
		char * end_ptr;
		long int value = strtoimax(ds->str, &end_ptr, 10);
		
		if(*end_ptr != '\0') { // Unsuccesful conversion.
			error = LEXICAL_ERROR;
			return;
		}
		if(value > INT_MAX) { // Check range error.
			error = LEXICAL_ERROR;
			return;
		}
		t->type = INT_LIT;
		
		/* Attach token attribute. */
		if(t_attach(t, ds->str)) {
			error = INTERNAL_ERROR;
		}
	} else {
			if(float_write(ds, c)) { // Write rest of float number.
				return; // Possible error is set in float_write.
			}

			/* Check if value is ok. */
			char * end_ptr;
			double value = strtod(ds->str, &end_ptr); // Try to convert value.

			if(*end_ptr != '\0') { // Unsuccesful conversion.
				error = LEXICAL_ERROR;
				return;
			}
			if(errno == ERANGE || value > __DBL_MAX__) { // Check for range error.
				error = LEXICAL_ERROR;
				return;
			}
			t->type = FLT_LIT;
			char dummy[50];
			sprintf(dummy, "%a", value); // Write in hexadecimal into dummy.
			
			/* Attach dummy as attribute. */
			if(t_attach(t, dummy)) {
				error = INTERNAL_ERROR;
			}
	}

}

void s_handler(dynamic_string_t * ds, token_t * t, int * c) {
	while( (*c = fgetc(stdin)) != EOF && *c != '"') { // Write string to ds.
		if(*c <= 32 || *c == 35) {
			/* Except unwriteable characters. */
			if(*c < 10) {
				if(ds_concat_str(ds, "\\00") || ds_write_uint(ds, *c)) {
					error = INTERNAL_ERROR;
					return;
				}	
			} else if(ds_concat_str(ds, "\\0") || ds_write_uint(ds, *c)) {
				error = INTERNAL_ERROR;
				return;
			}
			continue;
		}
		if(*c == '$') {
			error = LEXICAL_ERROR;
			return;
		}
		if(*c == '\\') { // Escape sequence.
			*c = fgetc(stdin);
			/* String interpolation. */
			switch(*c) {
				case 'n': if(ds_concat_str(ds, "\\010")) { error = INTERNAL_ERROR; return; } break;
				case 't': if(ds_concat_str(ds, "\\009")) { error = INTERNAL_ERROR; return; } break;
				case '"': if(ds_write(ds, '"')) { error = INTERNAL_ERROR; return; } break;
				case '\\': if(ds_concat_str(ds, "\\092")) { error = INTERNAL_ERROR; return; } break;
				case '$': if(ds_write(ds, '$')) { error = INTERNAL_ERROR; return; } break;
				case '0': case '1': case '2': case '3': if(oct_write(ds, c)) { error = INTERNAL_ERROR; return; } break; // Octal conversion of three characters.
				case 'x': if(hex_write(ds, c)) { error = INTERNAL_ERROR; return; } break;
				case EOF: error = LEXICAL_ERROR; break;
				default: 
					if(ds_concat_str(ds, "\\092")) { error = INTERNAL_ERROR; return; }
					ungetc(*c, stdin);
					break;
			}
		} else {
			if(ds_write(ds, *c)) {
				error = INTERNAL_ERROR;
				return;
			}
		}
	} // End write.
	if(*c != '"') {
		error = LEXICAL_ERROR;
		return;
	}

	/* Set type and attach associated value. */
	t->type = STR_LIT;
	if(t_attach(t, ds->str)) {
		error = INTERNAL_ERROR;
		return;
	}
}

void nte_handler(dynamic_string_t * ds, token_t * t, int * c) {
	static const char * reserved_words[] = {"?float", "?string", "?int"};
	static const int reserved_words_count = 3;

	*c = fgetc(stdin);
	if( *c == '>') {
		t->type = EPILOG; // '?>'
		*c = fgetc(stdin);
		if(*c != EOF) {
			error = LEXICAL_ERROR;
		}
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
	} while( (*c = fgetc(stdin)) != EOF && isalpha(*c));
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
		while((*c = fgetc(stdin)) != EOF && (*c == 'p' || *c == 'h')) { // Get prolog
			if(ds_write(ds, *c)) {
				error = INTERNAL_ERROR;
				return;
			}
		}
		ungetc(*c, stdin);
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

int oct_write(dynamic_string_t * ds, int * c) {
	dynamic_string_t * aux = ds_init();
	if(aux == NULL) {
		error = INTERNAL_ERROR;
		return 1;
	}
	if(ds_write(aux, *c)) { // Write first digit.
		error = INTERNAL_ERROR;
		ds_dstr(aux);
		return 1;
	}
	*c = fgetc(stdin); // Get and write second digit.
	if(ds_write(aux, *c)) {
		error = INTERNAL_ERROR;
		ds_dstr(aux);
		return 1;
	}
	*c = fgetc(stdin); // Get and write third digit.
	if(ds_write(aux, *c)) {
		error = INTERNAL_ERROR;
		ds_dstr(aux);
		return 1;
	}

	// Try octal conversion
	char * endptr;

	unsigned value = strtoumax(aux->str, &endptr, 8);
	if(*endptr != '\0') { // Conversion failed, insert as normal sequence.
		if(ds_concat_str(ds, "\\092")) { error = INTERNAL_ERROR; return 1; } // Insert escape sequence
		/* Return all characters back to stream for and escape sequence checking. */ 
		ungetc(aux->str[2], stdin);
		ungetc(aux->str[1], stdin);
		ungetc(aux->str[0], stdin); 
	} else {
		if(value >= 1 && value <= 255) {
			if(value <= 32 || value == 35 || value == 92) { // Check for special values.
				if(value < 10) {
					if(ds_concat_str(ds, "\\00") || ds_write_uint(ds, value)) {
						error = INTERNAL_ERROR;
						return 1; 
					} 
				} else if(ds_concat_str(ds, "\\0") || ds_write_uint(ds, value)) {
					error = INTERNAL_ERROR;
					return 1;
				}
			} else {
				ds_write(ds, value); // Write value directly...
				return 0;
			}
		} else {
			if(ds_concat_str(ds, "\\092")) { error = INTERNAL_ERROR; return 1; } // Insert escape sequence
			/* Return all characters back to stream for and escape sequence checking. */
			ungetc(aux->str[2], stdin);
			ungetc(aux->str[1], stdin);
			ungetc(aux->str[0], stdin); 
		}
	}
	ds_dstr(aux);

	return 0;
}


int hex_write(dynamic_string_t * ds, int * c) {
	dynamic_string_t * aux = ds_init();
	if(aux == NULL) {
		error = INTERNAL_ERROR;
		ds_dstr(aux);
		return 1;
	}
	*c = fgetc(stdin); // Get first digit.
	if(ds_write(aux, *c)) {
		error = INTERNAL_ERROR;
		ds_dstr(aux);
		return 1;
	}
	*c = fgetc(stdin); // Get second digit.
	if(ds_write(aux, *c)) {
		error = INTERNAL_ERROR;
		ds_dstr(aux);
		return 1;
	}

	// Try hex conversion
	char * endptr;

	unsigned value = strtoumax(aux->str, &endptr, 16);
	if(*endptr != '\0') {
		if(ds_concat_str(ds, "\\092x")) { error = INTERNAL_ERROR; return 1; }  // Insert escape sequence
		/* Return all characters back to stream for and escape sequence checking. */
		ungetc(aux->str[1], stdin);
		ungetc(aux->str[0], stdin);
	} else {
		if(value >= 1 && value <= 255) {
			if(value <= 32 || value == 35 || value == 92) { // Check for special values.
				if(value < 10) {
					if(ds_concat_str(ds, "\\00") || ds_write_uint(ds, value)) {
						error = INTERNAL_ERROR;
						return 1;
					}
				} else if(ds_concat_str(ds, "\\0") || ds_write_uint(ds, value)) {
					error = INTERNAL_ERROR;
					return 1;
				}
			} else {
				ds_write(ds, value); // Write value directly...
				return 0;
			}
		} else {
			if(ds_concat_str(ds, "\\092x")) { error = INTERNAL_ERROR; return 1; }  // Insert escape sequence
			/* Return all characters back to stream for and escape sequence checking. */
			ungetc(aux->str[1], stdin);
			ungetc(aux->str[0], stdin);
		}
	}
	ds_dstr(aux);
	return 0;
}

int float_write(dynamic_string_t * ds, int * c) {
	if(*c == '.') {
		ds_write(ds, *c);
		*c = fgetc(stdin);
		if(isdigit(*c)) {
			do {
				if(ds_write(ds, *c)) {
					error = INTERNAL_ERROR;
					return 1;
				}
			} while ( (*c = fgetc(stdin)) != EOF && (isdigit(*c)));
			if(*c == 'e' || *c == 'E') {
				if(ds_write(ds, *c)) { // Write char.
					error = INTERNAL_ERROR;
					return 1;
				}
				*c = fgetc(stdin);
				if(*c == '-' || *c == '+') {
					if(ds_write(ds, *c)) {
						error = INTERNAL_ERROR;
						return 1;
					}
					*c = fgetc(stdin);
					if(isdigit(*c)) {
						do {
							if(ds_write(ds, *c)) {
								error = INTERNAL_ERROR;
								return 1;
							}
						} while ( (*c = fgetc(stdin)) != EOF && (isdigit(*c)));
						ungetc(*c, stdin);
						return 0;
					} else {
						error = LEXICAL_ERROR;
						return 1;
					}
				} else if (isdigit(*c)) {
					do {
						if(ds_write(ds, *c)) {
							error = INTERNAL_ERROR;
							return 1;
						}
					} while ( (*c = fgetc(stdin)) != EOF && (isdigit(*c)));
					ungetc(*c, stdin);
					return 0;
				} else {
					error = LEXICAL_ERROR;
					return 1;
				}
			} else {
				ungetc(*c, stdin);
				return 0;
			}
		} else {
			error = LEXICAL_ERROR;
			return 1;
		}
	} else if (*c == 'e' || *c == 'E') {
		if(ds_write(ds, *c)) { // Write char.
			error = INTERNAL_ERROR;
			return 1;
		}
		*c = fgetc(stdin);
		if(*c == '-' || *c == '+') {
			if(ds_write(ds, *c)) {
				error = INTERNAL_ERROR;
				return 1;
			}
			*c = fgetc(stdin);
			if(isdigit(*c)) {
				do {
					if(ds_write(ds, *c)) {
						error = INTERNAL_ERROR;
						return 1;
					}
				} while ( (*c = fgetc(stdin)) != EOF && (isdigit(*c)));
				ungetc(*c, stdin);
				return 0;
			} else {
				error = LEXICAL_ERROR;
				return 1;
			}
		} else if (isdigit(*c)) {
			do {
					if(ds_write(ds, *c)) {
						error = INTERNAL_ERROR;
						return 1;
					}
			} while ( (*c = fgetc(stdin)) != EOF && (isdigit(*c)));
			ungetc(*c, stdin);
			return 0;
		} else {
			error = LEXICAL_ERROR;
			return 1;
		}
	}
	return 0;
}
