#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "precedence_parser.h"
#include "error.h"
#include "scanner.h"

void print_passed(){
	printf("\033[0;32m");
    printf("TEST PASSED\n\033[0m");
}

void print_failed(){
	printf("\033[0;31m");
    printf("TEST FAILED\n\033[0m");
}

void test_precedence_parser(char* expression, error_codes_t expected_error_code){
	printf("===================================\n");
	FILE* fp = fopen("./tests/precedence_parser/test_in.php","w+");
	if(fp == NULL) exit(1);
	fprintf(fp, "%s", expression);
	fclose(fp);

	freopen("./tests/precedence_parser/test_in.php", "r", stdin);

	printf("INPUT: %s\n", expression);

	token_t* t;
	token_array_t* ta = token_array_create();
	if(ta == NULL){
		printf("Allocation error\n");
		exit(1);
	} 
	
	error = OK;
    while((t = get_token()) != NULL && t->type != END){
		if(token_array_push_token(ta, t) != 0){
			printf("Allocation error\n");
			exit(1);
		} 
	}
	if(t != NULL) t_dstr(t);

	printf("TINPUT: ");
	token_array_expr_print(ta, stdout);
	printf("\n");
	
	token_array_t* postfix_result = parse_expression(ta);

	printf("OUTPUT: ");
	token_array_expr_print(postfix_result, stdout);
	printf("\n");

	if(expected_error_code == error){
		print_passed();
	}
	else{
		print_failed();
		printf("EXPECTED ERROR CODE:");
		printf("%d\n", expected_error_code);
		printf("GOT:");
		printf("%d", error);
		printf("\n");
	}

	token_array_free(postfix_result);
}

int main(){
	test_precedence_parser("3", OK);
	test_precedence_parser("3+4", OK);
	test_precedence_parser("1*2/3*4", OK);
	test_precedence_parser("1<2", OK);
	test_precedence_parser("1<2<3", SYNTAX_ERROR);
	test_precedence_parser("1<2===3<4", OK);
	test_precedence_parser("1===2<3===4", SYNTAX_ERROR);
	test_precedence_parser("1<2===3<4===5", SYNTAX_ERROR);
	test_precedence_parser("", OK);
	test_precedence_parser("1<<5", SYNTAX_ERROR);
	test_precedence_parser("1<", SYNTAX_ERROR);
	test_precedence_parser("*", SYNTAX_ERROR);
	test_precedence_parser(")(", SYNTAX_ERROR);
	test_precedence_parser("()", SYNTAX_ERROR);
	test_precedence_parser("1*(2+3)", OK);
	test_precedence_parser("-3", SYNTAX_ERROR);
	test_precedence_parser("1+2-\"3\".\"4\"*(6+7/8)", OK);
	test_precedence_parser("\"a\"+2", SYNTAX_ERROR);
	test_precedence_parser("null+3", SYNTAX_ERROR);
}