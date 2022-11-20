/****
 ** expgen.c
 ** Řešení IFJ-PROJEKT, 13.11.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file expgen.c
 * @author xhorva17
 * @brief Expression generation implementation.
 * @date 2022-13-11
 */

#include <stdio.h>
#include <stdbool.h>
#include "expgen.h"
#include "token_array.h"
#include "generator.h"
#include "labelgen.h"
#include "scanner.h"
#include "error.h"
#include "precedence_parser.h"
#include "abstract_syntax_tree.h"

/* Global variables */
char * lhs = "GF@_lhs";
char * rhs = "GF@_rhs";
// char * res = "GF@_res";
char * tlhs = "GF@_tlhs";
char * trhs = "GF@_trhs";

int main() {
    token_array_t * tarr;
    tarr = get_exp("null*2+3");
    // printf("OUTPUT: ");
	// token_array_expr_print(tarr, stdout);
    G(".IFJcode22");
    G("DEFVAR %s", lhs);
    G("DEFVAR %s", rhs);
    G("DEFVAR %s", tlhs);
    G("DEFVAR %s", trhs);
	printf("\n\n\n");
    generator_context_t * gc = generator_context_create();
    if(gc == NULL) {
        return 1;
    }
    gen_expression(tarr, gc);


    G("WRITE GF@_lhs");
    G("WRITE string@\\010");

    token_array_free(tarr);



}

void gen_expression(token_array_t * tarr, generator_context_t * gc) {
    char * scope_label;
    operation_t operation;
    if(gc->is_in_function) {
        scope_label = "LF@";
    } else {
        scope_label = "GF@";
    }
    for(unsigned i = 0; i < tarr->token_count; i++) {
        token_t * token = tarr->array[i];
        if(isoperand(token)) {
            push_operand(token, scope_label);
        } else { // Is operator
            char * execute_label = gen_label(NULL, NULL, "?execute", true);
            if(execute_label == NULL) {
                error = INTERNAL_ERROR;
                return;
            }
            set_operator(&operation, token);
            pop_operand(operation, lhs, tlhs);
            pop_operand(operation, rhs, trhs);
            check_types(operation, execute_label);
            execute_operation(operation, token->type, execute_label);
        }
    }
}

void push_operand(token_t * token, const char * scope_label) {
    char * label;
    switch(token->type) {
        case VAR_ID: label = gen_label(scope_label, token->aval, NULL, false); break;
        case FLT_LIT: label = gen_label("float@", token->aval, NULL, false); break;
        case STR_LIT: label = gen_label("string@", token->aval, NULL, false); break;
        case INT_LIT: label = gen_label("int@", token->aval, NULL, false); break;
        case NULL_LIT: label = gen_label("nil@nil", NULL, NULL, false); break;
        default: break;
    }
    G("PUSHS %s", label);
}

void set_operator(operation_t * operation, token_t * token) {
    switch(token->type) {
        case MUL: case ADD: case SUB:                               *operation = ARITHMETIC; break;
        case LT: case GT: case GTE: case LTE: case EQ: case NEQ:    *operation = RELATIONAL; break;
        case DIV:                                                   *operation = DIVISION; break;
        case CONCAT:                                                *operation = STRING; break;
        default: break;
    }
}

void pop_operand(operation_t operation, const char * operand, const char * operand_type) {
    // G("\n# POPPING OPERAND");
    G("POPS %s", operand); // Pop operand to global variable.
    if(operation == ARITHMETIC) {
        char * type_ok;
        char * skip_nil_conv;
        if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
            error = INTERNAL_ERROR;
            return;
        }
        G("TYPE %s %s", operand_type, operand);
        G("JUMPIFNEQ %s %s string@string", type_ok, operand_type);
        G("EXIT int@7");
        G("LABEL %s", type_ok);
        G("JUMPIFNEQ %s %s string@nil", skip_nil_conv, operand_type);
        G("MOVE %s int@0", operand); // nil to int conversion
        G("LABEL %s", skip_nil_conv);
        G("TYPE %s %s", operand_type, operand);
    }
    // G("# END POP\n");
}

void check_types(operation_t operation, char * execute_label) {
    // G("### CHECKING OPERAND TYPES");
    if(operation == ARITHMETIC) {
        char * convert_rhs = gen_label(NULL, NULL, "?convert_rhs", true);
        G("JUMPIFEQ %s %s %s", execute_label, tlhs, trhs);
        G("JUMPIFEQ %s %s string@float", convert_rhs, tlhs);
        G("INT2FLOAT %s %s", lhs, lhs);
        G("JUMP %s", execute_label);
        G("LABEL %s", convert_rhs);
        G("INT2FLOAT %s %s", rhs, rhs);
    }
}

void execute_operation(operation_t operation, token_type_t operator, char * execute_label) {
    // G("### EXECUTE");
    if(operation == ARITHMETIC) {
        G("LABEL %s", execute_label);
        switch(operator) {
            case MUL: G("MUL %s %s %s", lhs, lhs, rhs); break;
            case SUB: G("SUB %s %s %s", lhs, lhs, rhs); break;
            case ADD: G("ADD %s %s %s", lhs, lhs, rhs); break;
            default: break;
        }
        G("PUSHS %s", lhs); // Result write back
    }
    // G("### END OPERATION");
}











bool isoperand(token_t * token) {
    switch(token->type) {
        case VAR_ID: case STR_LIT: case FLT_LIT: case INT_LIT: case NULL_LIT: return true; break;
        default: return false; break;
    }

}

bool isoperator(token_t * token) {
    switch(token->type) {
        case VAR_ID: case STR_LIT: case FLT_LIT: case INT_LIT: return false; break;
        default: return true; break;
    }
}



token_array_t * get_exp(char* expression){
	FILE* fp = fopen("./tests/expgen/test_in.php","w+");
	if(fp == NULL) exit(1);
	fprintf(fp, "%s", expression);
	fclose(fp);

	freopen("./tests/expgen/test_in.php", "r", stdin);
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
	
	return parse_expression(ta);
}
