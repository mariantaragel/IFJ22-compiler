/****
 ** token.c
 ** Řešení IFJ-PROJEKT, 7.10.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file token.c
 * @author xhorva17
 * @brief Token implementation.
 * @date 2022-10-07
 */

#include <stdio.h>
#include <stdlib.h>
#include "token.h"


token_t * t_init() {
	token_t * t = (token_t*) calloc(1, sizeof(token_t));
	if(t == NULL) {
		return NULL;
	}
	return t;
}

// TODO: Deallocate associated values.
void t_dstr(token_t * t) {
	if(t->type == FUNC_ID)
		free(t->sval);
	free(t);
	return;
}

void t_print(token_t * t) {
	printf("TOKEN: {\n");
	if(t->type == FUNC_ID) {
		printf("\ttype: FUNC_ID\n");
	} else if(t->type == VAR_ID) {
		printf("\ttype: VAR_ID\n");
	} else if(t->type == STR_LIT) {
		printf("\ttype: STR_LIT\n");
	} else if(t->type == INT_LIT) {
		printf("\ttype: INT_LIT\n");
	} else if(t->type == FLT_LIT) {
		printf("\ttype: FLT_LIT\n");
	} else if(t->type == MUL) {
		printf("\ttype: MUL\n");
	} else if(t->type == DIV) {
		printf("\ttype: DIV\n");
	} else if(t->type == ADD) {
		printf("\ttype: ADD\n");
	} else if(t->type == SUB) {
		printf("\ttype: SUB\n");
	} else if(t->type == CONCAT) {
		printf("\ttype: CONCAT\n");
	} else if(t->type == LT) {
		printf("\ttype: LT\n");
	} else if(t->type == GT) {
		printf("\ttype: GT\n");
	} else if(t->type == LTE) {
		printf("\ttype: LTE\n");
	} else if(t->type == GTE) {
		printf("\ttype: GTE\n");
	} else if(t->type == EQ) {
		printf("\ttype: EQ\n");
	} else if(t->type == NEQ) {
		printf("\ttype: NEQ\n");
	} else if(t->type == COMMA) {
		printf("\ttype: COMMA\n");
	} else if(t->type == COLON) {
		printf("\ttype: COLON\n");
	} else if(t->type == SCOLON) {
		printf("\ttype: SCOLON\n");
	} else if(t->type == LB) {
		printf("\ttype: LB\n");
	} else if(t->type == RB) {
		printf("\ttype: RB\n");
	} else if(t->type == LCB) {
		printf("\ttype: LCB\n");
	} else if(t->type == RCB) {
		printf("\ttype: RCB\n");
	} else if(t->type == ASSIGN) {
		printf("\ttype: ASSIGN\n");
	} else if(t->type == FLT_T) {
		printf("\ttype: FLT_T\n");
	} else if(t->type == STR_T) {
		printf("\ttype: STR_T\n");
	} else if(t->type == INT_T) {
		printf("\ttype: INT_T\n");
	} else if(t->type == VOID_T) {
		printf("\ttype: VOID_T\n");
	} else if(t->type == NULL_LIT) {
		printf("\ttype: NULL_LIT\n");
	} else if(t->type == NFLT_T) {
		printf("\ttype: NFLT_T\n");
	} else if(t->type == NINT_T) {
		printf("\ttype: NINT_T\n");
	} else if(t->type == NSTR_T) {
		printf("\ttype: NSTR_T\n");
	} else if(t->type == ELSE) {
		printf("\ttype: ELSE\n");
	} else if(t->type == FUNCTION) {
		printf("\ttype: FUNCTION\n");
	} else if(t->type == IF) {
		printf("\ttype: IF\n");
	} else if(t->type == RETURN) {
		printf("\ttype: RETURN\n");
	} else if(t->type == WHILE) {
		printf("\ttype: WHILE\n");
	} else if(t->type == PROLOG) {
		printf("\ttype: PROLOG\n");
	} else if(t->type == EPILOG) {
		printf("\ttype: EPILOG\n");
	} else if(t->type == END) {
		printf("\ttype: END\n");
	}
	if(t->type == FUNC_ID || t->type == VAR_ID || t->type == STR_LIT) {
		printf("\tAssociated value: \"%s\"\n", t->sval);
	}
	if(t->type == INT_LIT) {
		printf("\tAssociated value: %d\n", t->ival);
	}
	if(t->type == FLT_LIT) {
		printf("\tAssociated value: %lf\n", t->fval);
	}
	printf("}\n");
}

