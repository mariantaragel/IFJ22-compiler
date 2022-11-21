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
#include "generator_tools.h"
#include "expgen.h"
#include "token_array.h"
#include "generator.c"
#include "scanner.h"
#include "error.h"
#include "precedence_parser.h"

/* Global variables */
char * lhs = "GF@_lhs";
char * rhs = "GF@_rhs";
char * tlhs = "GF@_tlhs";
char * trhs = "GF@_trhs";

int main() {
    G(".IFJcode22");
    G("DEFVAR %s", lhs);
    G("DEFVAR %s", rhs);
    G("DEFVAR %s", tlhs);
    G("DEFVAR %s", trhs);
    G("JUMP program_start");
    G("\n\n");
    gen_operation_functions();


    G("LABEL program_start");


    token_array_t * tarr;
    tarr = get_exp("null < 2");
    
	printf("\n\n\n");
    generator_context_t * gc = generator_context_create();
    if(gc == NULL) {
        return 1;
    }
    gen_expression(tarr, gc);

    G("BREAK");
    G("WRITE GF@_lhs");
    G("WRITE string@\\010");

    token_array_free(tarr);


}

void gen_operation_functions() {
    gen_arithmetic_operation(ADD);
    gen_arithmetic_operation(MUL);
    gen_arithmetic_operation(SUB);
    gen_string_operation(CONCAT);
    gen_equality_operation(EQ);
    gen_equality_operation(NEQ);
    gen_ltgt_operation(LT);
    gen_ltgt_operation(GT);
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
            switch(token->type) {
                case ADD: G("CALL %%add_op"); break;
                case MUL: G("CALL %%mul_op"); break;
                case SUB: G("CALL %%sub_op"); break;
                // case DIV: G("CALL %%div_op"); break;
                case CONCAT: G("CALL %%concat_op"); break;
                case EQ: G("CALL %%eq_op"); break;
                case NEQ: G("CALL %%neq_op"); break;
                case LT: G("CALL %%lt_op"); break;
                case GT: G("CALL %%gt_op"); break;
                // case GTE: G("CALL %%gte_op"); break;
                // case LTE: G("CALL %%lte_op"); break;
                default: break;
            }
            // char * execute_label = gen_label(NULL, NULL, "?execute", true);
            // if(execute_label == NULL) {
            //     error = INTERNAL_ERROR;
            //     return;
            // }
            // set_operator(&operation, token);
            // pop_operand(operation, lhs, tlhs);
            // pop_operand(operation, rhs, trhs);
            // check_types(operation, execute_label);
            // execute_operation(operation, token->type, execute_label);
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
        case LT: case GT:                                           *operation = LTGT; break;
        case LTE: case GTE:                                         *operation = LTEGTE; break;
        case EQ: case NEQ:                                          *operation = EQUALITY; break;
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
        G("WRITE int@7"); // DEBUGGING ONLY!
        G("EXIT int@7");
        G("LABEL %s", type_ok);
        G("JUMPIFNEQ %s %s string@nil", skip_nil_conv, operand_type);
        G("MOVE %s int@0", operand); // nil to int conversion
        G("LABEL %s", skip_nil_conv);
        G("TYPE %s %s", operand_type, operand);

    } else if (operation == STRING) {
        char * type_ok;
        char * skip_nil_conv;
        if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
            error = INTERNAL_ERROR;
            return;
        }
        G("TYPE %s %s", operand_type, operand);
        G("JUMPIFNEQ %s %s string@float", type_ok, operand_type);
        G("JUMPIFNEQ %s %s string@int", type_ok, operand_type);
        G("WRITE int@7"); // DEBUGGING ONLY!
        G("EXIT int@7");
        G("LABEL %s", type_ok);
        G("JUMPIFNEQ %s %s string@nil", skip_nil_conv, operand_type);
        G("MOVE %s string@", operand);
        G("LABEL %s", skip_nil_conv);
        G("TYPE %s %s", operand_type, operand);

    } else if (operation == EQUALITY) {
        G("TYPE %s %s", operand_type, operand);
    } else if (operation == LTGT) {
        G("TYPE %s %s", operand_type, operand);
    }
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
    } else if (operation == STRING) {
        G("CONCAT %s %s %s", lhs, lhs, rhs);
        G("PUSHS %s", lhs);
    } else if (operation == EQUALITY) {
        char * end_label = gen_label(NULL,NULL,"?end_op", true);
        G("JUMPIFEQ %s %s %s", execute_label, tlhs, trhs);
        switch(operator) {
            case EQ: G("MOVE %s bool@false", lhs); break;
            case NEQ: G("MOVE %s bool@true", lhs); break;
            default: break;
        }
        G("JUMP %s", end_label);
        G("LABEL %s", execute_label);
        G("EQ %s %s %s", lhs, lhs, rhs);
        if(operator == NEQ) {
            G("NOT %s %s", lhs, lhs);
        }
        G("LABEL %s", end_label);
        G("PUSHS %s", lhs);
    } else if (operation == LTGT) {
        char * end_label = gen_label(NULL,NULL,"?end_op", true);
        char * skip_lhs_cast_label = gen_label(NULL,NULL,"?skip_lhs_cast", true);
        char * skip_rhs_cast_label = gen_label(NULL,NULL,"?skip_rhs_cast", true);
        char * lhs_ok_label = gen_label(NULL, NULL, "?lhs_ok", true);
        char * rhs_ok_label = gen_label(NULL, NULL, "?rhs_ok", true);
        char * lhs_nil_label = gen_label(NULL, NULL, "?lhs_nil", true);
        char * rhs_nil_label = gen_label(NULL, NULL, "?rhs_nil", true);


        G("JUMPIFNEQ %s %s string@nil",lhs_nil_label, tlhs); // If nil, exit.
        G("MOVE %s bool@false", lhs);
        G("JUMP %s", end_label);
        G("LABEL %s", lhs_nil_label);
        G("JUMPIFNEQ %s %s string@nil", rhs_nil_label, trhs); // If nil, exit.
        G("MOVE %s bool@false", lhs);
        G("JUMP %s", end_label);
        G("LABEL %s", rhs_nil_label);

        /* LABELS -> rhs_ok, lhs_ok */
        G("JUMPIFEQ %s %s %s", execute_label, tlhs, trhs);
        G("JUMPIFNEQ %s %s string@string", lhs_ok_label, tlhs);
        G("EXIT int@7");
        G("LABEL %s", lhs_ok_label);
        G("JUMPIFNEQ %s %s string@string", rhs_ok_label, trhs);
        G("EXIT int@7");
        G("LABEL %s", rhs_ok_label);
        /* Execute label, skip_lhs_cast, skip_rhs_cast, */

        G("JUMPIFNEQ %s %s string@int", skip_lhs_cast_label,tlhs); // No type cast.
        G("INT2FLOAT %s %s", lhs, lhs);
        G("LABEL %s", skip_lhs_cast_label);
        G("JUMPIFNEQ %s %s string@int", skip_rhs_cast_label, trhs);
        G("INT2FLOAT %s %s", rhs, rhs);
        G("LABEL %s", skip_rhs_cast_label);
        G("LABEL %s", execute_label);
        switch(operator) {
            case LT: G("LT %s %s %s", lhs, lhs, rhs); break; // lhs <- lhs < rhs
            case GT: G("GT %s %s %s", lhs, lhs, rhs); break; // lhs <- lhs > rhs
            default: break;
        }
        G("LABEL %s", end_label);
        G("PUSHS %s", lhs);
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
        case VAR_ID: case STR_LIT: case FLT_LIT: case INT_LIT: case NULL_LIT: return false; break;
        default: return true; break;
    }
}

void gen_arithmetic_operation(token_type_t operation) {
    switch(operation) {
        case ADD: G("LABEL %%add_op"); break;
        case MUL: G("LABEL %%mul_op"); break;
        case SUB: G("LABEL %%sub_op"); break;
        default: break;
    }
    char * execute_label = gen_label(NULL, NULL, "?execute", true);
    if(execute_label == NULL) {
        error = INTERNAL_ERROR;
        return;
    }
    pop_operand(ARITHMETIC, lhs, tlhs);
    pop_operand(ARITHMETIC, rhs, trhs);
    check_types(ARITHMETIC, execute_label);
    execute_operation(ARITHMETIC, operation, execute_label);
    G("RETURN");
    G("\n\n");
}

// void gen_division_operation();

void gen_equality_operation(token_type_t operation) {
    switch(operation) {
        case EQ: G("LABEL %%eq_op"); break;
        case NEQ: G("LABEL %%neq_op"); break;
        default: break;
    }
    char * execute_label = gen_label(NULL, NULL, "?execute", true);
    if(execute_label == NULL) {
        error = INTERNAL_ERROR;
        return;
    }
    pop_operand(EQUALITY, lhs, tlhs);
    pop_operand(EQUALITY, rhs, trhs);
    check_types(EQUALITY, execute_label);
    execute_operation(EQUALITY, operation, execute_label);
    G("RETURN");
    G("\n\n");
}

void gen_string_operation(token_type_t operation) {
    G("LABEL %%concat_op");
    char * execute_label = gen_label(NULL, NULL, "?execute", true);
    if(execute_label == NULL) {
        error = INTERNAL_ERROR;
        return;
    }
    pop_operand(STRING, lhs, tlhs);
    pop_operand(STRING, rhs, trhs);
    check_types(STRING, execute_label);
    execute_operation(STRING, operation, execute_label);
    G("RETURN");
    G("\n\n");
}

void gen_ltgt_operation(token_type_t operation) {
    switch(operation) {
        case LT: G("LABEL %%lt_op"); break;
        case GT: G("LABEL %%gt_op"); break;
        default: break;
    }
    char * execute_label = gen_label(NULL, NULL, "?execute", true);
    if(execute_label == NULL) {
        error = INTERNAL_ERROR;
        return;
    }
    pop_operand(LTGT, lhs, tlhs);
    pop_operand(LTGT, rhs, trhs);
    check_types(LTGT, execute_label);
    execute_operation(LTGT, operation, execute_label);
    G("RETURN");
    G("\n\n");
}

// void gen_ltegte_operation();


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
