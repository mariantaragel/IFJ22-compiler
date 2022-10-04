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
	lexeme_t lexeme = lex_init();
	if(lexeme.lex == NULL) {
		//TODO: COMPILER MEMORY ERROR.
		return NULL;
	}
	int c = 0;
	
	do { 
	/* Eat up leading whitespace. TODO: EOF checks if EOF found?? */
		continue;
	} while( (c = fgetc(stdin)) != EOF && isspace(c) != 0); // watchout for file.

	if(c == EOF) {
		lex_dstr(lexeme);
		return NULL;
	}
	
	switch(c) {
		/* */
		case letter case '_': vik_handler(&c, &lexeme); break;
		case digit printf("digit or real number"); break;
		case '$': vik_handler(&c, &lexeme); break;
		case '"': s_handler(&c, &lexeme); break;
		
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
	lex_dstr(lexeme);
	return NULL;
	
}

/* return keyword token in case of match on identifier. */
// void match_keyword();

void vik_handler(int * c, lexeme_t * lexeme) {
	do {
		lexeme->lex[lexeme->wi] = *c;
		lexeme->wi++;
		if(lexeme->wi == lexeme->size + 1) { // Resize in case size limit is reached...
			*lexeme = lex_resize(*lexeme);
		}
	} while( (*c = fgetc(stdin)) != EOF && (isalnum(*c) != 0 || *c == '_') && isspace(*c) == 0); // watchout for file.
	ungetc(*c, stdin);
	printf("%s\n", lexeme->lex);
}

void s_handler(int * c, lexeme_t * lexeme) {
	printf("String start c = '%c'\n", *c);
	while( (*c = fgetc(stdin)) != EOF && *c != '"') {
		lexeme->lex[lexeme->wi] = *c;
		lexeme->wi++;
		if(lexeme->wi == lexeme->size + 1) {
			*lexeme = lex_resize(*lexeme);
		}
	}
	printf("%s\n", lexeme->lex);
	printf("String start c = '%c'\n", *c);
}



lexeme_t lex_init() {
	lexeme_t lexeme;
	lexeme.size = 100;
	lexeme.wi = 0;
	lexeme.lex  = (char*) calloc(1,lexeme.size*sizeof(char));
	if(lexeme.lex == NULL) {
		// TODO: COMPILER MEMORY ERROR.
		return lexeme;
	}
	return lexeme;
}

void lex_dstr(lexeme_t lexeme) {
	free(lexeme.lex);
}

lexeme_t lex_resize(lexeme_t lexeme) {
	char * new_lex = (char*) realloc((void*) lexeme.lex, lexeme.size + 100);
	if(new_lex == NULL) {
		// TODO: COMPILER MEMORY ERROR.
		return lexeme; // Correct?
	}
	lexeme.size += 100;
	return lexeme;
}

