/****
 ** children.h
 ** Řešení IFJ-PROJEKT, 01.10.2022
 ** Autor: 
 ** Přeloženo:
 **/

/**
 * @file scanner.c
 * @author 
 * @brief Scanner function implementations.
 * @date 2022-10-01
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "scanner.h"
#include "dynamic_string.h"

int main() {
	token_t * token;
	token = get_token();
	token = get_token();
	token = get_token();
	
	if(token)
		return 1;
	return 0;
}



// TODO: Lexer error handling.
token_t * get_token() {
	dynamic_string_t * ds = ds_init();
	if(ds == NULL) {
		//TODO: COMPILER MEMORY ERROR.
		return NULL;
	}
	int c = 0;
	
	do { 
	/* Eat up leading whitespace. TODO: EOF checks if EOF found?? */
		continue;
	} while( (c = fgetc(stdin)) != EOF && isspace(c) != 0); // watchout for file.

	if(c == EOF) {
		ds_dstr(ds);
		return NULL;
	}
	// e
	switch(c) {
		/* */
		case letter case '_': vik_handler(ds, &c); break;
		case digit printf("digit or real number"); break;
		case '$': vik_handler(ds, &c); break;
		case '"': s_handler(ds, &c); break;
		
		/* ARITHMETIC TOKENS */
		case '+': printf("add"); break;
		case '-': printf("minus"); break;
		case '*': printf("mul"); break;
		case '/': printf("division or comment"); break;
		
		/* PUNCTUATION */
		case ',': printf("comma"); break;
		case ':': printf("colon"); break;
		case ';': printf("semicolon"); break;
		case '(': printf("left bracket"); break;
		case ')': printf("right bracket"); break;
		case '{': printf("left curly bracket"); break;
		case '}': printf("right curly bracket"); break;
	
		/* RELATIONAL */
		case '=': printf("assignment or equality"); break;
		case '!': printf("negation"); break;
		case '<': printf("lt or lte"); break;
		case '>': printf("gt or gte"); break;
		
		default: fprintf(stderr, "err"); break;
	}
	ds_dstr(ds);
	return NULL;
	
}

/* return keyword token in case of match on identifier. */
// void match_keyword();

void vik_handler(dynamic_string_t * ds, int * c) {
	do {
		if(ds_write(ds, *c)) { // Write character.
			//TODO: Internal compiler error.
			return;
		}
	} while( (*c = fgetc(stdin)) != EOF && (isalnum(*c) != 0 || *c == '_') && isspace(*c) == 0); // watchout for file.
	ungetc(*c, stdin);
	printf("%s\n", ds->str);
}

void s_handler(dynamic_string_t * ds, int * c) {
	printf("String start c = '%c'\n", *c);
	while( (*c = fgetc(stdin)) != EOF && *c != '"') {
		if(ds_write(ds, *c)) {
			//TODO: Internal compiler error.
			return;
		}
	}
	printf("%s\n", ds->str);
	printf("String start c = '%c'\n", *c);
}

