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


// Comment out main when not testing.
int main() {
	token_t * token;
	FILE * fp = fopen("test.php", "r");
	if(fp == NULL) {
		fprintf(stderr, "file error");
		return 1;
	}
	token = get_token(fp);
	token = get_token(fp);
	token = get_token(fp);
	
	/*
	int i = 48;
	while(i != 58) {
		printf("case %d: ", i);
		i++;
	}
	*/
	if(token)
		return 1;
	return 0;
}

// TODO: Lexer error handling.
token_t * get_token(FILE * fp) {
	lexeme_t lexeme = lex_init();
	if(lexeme.lex == NULL) {
		//TODO: COMPILER MEMORY ERROR.
		return NULL;
	}
	int c = 0;
	
	do { // Eat up all leading whitespace.
		//printf("Skipping \"%c\"", c);
		continue;
	} while( (c = fgetc(fp)) != EOF && isspace(c) != 0); // watchout for file.
	// TODO: EOF checks?
	
	//printf("first char->%c\n", c);
	// ! CHECK MACROS IN scanner.h before changing case statements.
	switch(c) { // Identify beginning of tokens based on regexes...
		/* */
		case letter case '_': vik_handler(&c, fp, &lexeme); break;
		case digit printf("digit or real number"); break;
		case '$': vik_handler(&c, fp, &lexeme); break;
		case '"': s_handler(&c, fp, &lexeme); break;
		
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

void vik_handler(int * c, FILE * fp, lexeme_t * lexeme) {
	do {
		lexeme->lex[lexeme->wi] = *c;
		lexeme->wi++;
	} while( (*c = fgetc(fp)) != EOF && (isalnum(*c) != 0 || *c == '_') && isspace(*c) == 0); // watchout for file.
	ungetc(*c, fp);
	printf("%s\n", lexeme->lex);
	// keyword matching...
}

void s_handler(int * c, FILE * fp, lexeme_t * lexeme) { // dispose of '"'
	do {
		lexeme->lex[lexeme->wi] = *c;
		lexeme->wi++;
		// lexeme size checking... if wi == 100 ... resize lexeme...
	} while( (*c = fgetc(fp)) != EOF && *c != '"'); // watchout for file.
	printf("%s\n", lexeme->lex);
	//printf("%s\n", lexeme->lex);
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

