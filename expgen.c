/**
 * @file expgen.c
 * @author Martin Horvat, xhorva17
 * @brief Expression generation implementation.
 * @date 2022-13-11
 */

#include <stdio.h>
#include <stdbool.h>
#include "generator_tools.h"
#include "expgen.h"
#include "token_array.h"
#include "scanner.h"
#include "error.h"
#include "precedence_parser.h"

/* Global variables for generator. Don't change! */
const char * lhs = "GF@_lhs";
const char * rhs = "GF@_rhs";
const char * tlhs = "GF@_tlhs";
const char * trhs = "GF@_trhs";
const char * aux1 = "GF@_aux1";

typedef enum {ARITHMETIC, DIVISION, STRING, EQUALITY, LTGT, LTEGTE} operation_t;
void push_operand(const token_t * token, char * scope_label);
bool isoperand(const token_t * token);
void gen_arithmetic_operation(const token_type_t operation);
void pop_operand_arithmetic(const char * operand, const char * operand_type);
void gen_division_operation();
void pop_operand_division(const char * operand, const char * operand_type);
void gen_string_operation();
void pop_operand_string(const char * operand, const char * operand_type);
void gen_equality_operation(const token_type_t operation);
void pop_operand_equality(const char * operand, const char * operand_type);
void gen_ltgt_operation(const token_type_t operation);
void pop_operand_ltgt(const char * operand, const char * operand_type);
void gen_ltegte_operation(const token_type_t operation);
void pop_operand_ltegte(const char * operand, const char * operand_type);

/* Helper functions */
// token_array_t * get_exp(char* expression);

/* Testing main. Makefile -> make expgen*/
// int main() {
//     G(".IFJcode22");
//     G("DEFVAR %s", lhs);
//     G("DEFVAR %s", rhs);
//     G("DEFVAR %s", tlhs);
//     G("DEFVAR %s", trhs);
//     G("DEFVAR %s", aux1);
//     G("JUMP program_start");
//     G("\n\n");
//     gen_operation_functions();
//     G("LABEL program_start");
//     token_array_t * tarr;
//     tarr = get_exp("\"\" <= null");
// 	   printf("\n\n\n");
    
//     generator_context_t * gc = generator_context_create();
//     if(gc == NULL) {
//         return 1;
//     }
//     gen_expression(tarr, gc);
//     G("BREAK");
//     token_array_free(tarr);
//     generator_context_free(gc);
// }

void gen_operation_functions() {
    gen_arithmetic_operation(ADD);
    gen_arithmetic_operation(MUL);
    gen_arithmetic_operation(SUB);
    gen_division_operation();
    gen_string_operation();
    gen_equality_operation(EQ);
    gen_equality_operation(NEQ);
    gen_ltgt_operation(LT);
    gen_ltgt_operation(GT);
    gen_ltegte_operation(LTE);
    gen_ltegte_operation(GTE);
} // gen_operation_functions, OK

void gen_expression(token_array_t * tarr, generator_context_t * gc) {
    char * scope_label;
    if(gc->is_in_function) { // Set scope label.
        scope_label = "LF@";
    } else {
        scope_label = "GF@";
    }
    for(unsigned i = 0; i < tarr->token_count; i++) { // Parse expression.
        token_t * token = tarr->array[i];
        if(isoperand(token)) { // If operand, push onto stack.
            push_operand(token, scope_label);
        } else { // If operator, perform operation on two last pushed operands.
            switch(token->type) {
                case ADD: G("CALL &add_op"); break;
                case MUL: G("CALL &mul_op"); break;
                case SUB: G("CALL &sub_op"); break;
                case DIV: G("CALL &div_op"); break;
                case CONCAT: G("CALL &concat_op"); break;
                case EQ: G("CALL &eq_op"); break;
                case NEQ: G("CALL &neq_op"); break;
                case LT: G("CALL &lt_op"); break;
                case GT: G("CALL &gt_op"); break;
                case GTE: G("CALL &gte_op"); break;
                case LTE: G("CALL &lte_op"); break;
                default: break;
            }
        }
    }
} // gen_expression, OK

void push_operand(const token_t * token, char * scope_label) {
    char * label;
    /* Generate label for push based on token type. */
    switch(token->type) {
        case VAR_ID: label = gen_label(scope_label, token->aval, NULL, false); break;
        case FLT_LIT: label = gen_label("float@", token->aval, NULL, false); break;
        case STR_LIT: label = gen_label("string@", token->aval, NULL, false); break;
        case INT_LIT: label = gen_label("int@", token->aval, NULL, false); break;
        case NULL_LIT: label = gen_label("nil@nil", NULL, NULL, false); break;
        default: break;
    }
    G("PUSHS %s", label);
    free(label);
} // push_operand, OK

bool isoperand(const token_t * token) {
    switch(token->type) {
        case VAR_ID: case STR_LIT: case FLT_LIT: case INT_LIT: case NULL_LIT: return true; break;
        default: return false; break;
    }

} // isoperand, OK

void gen_arithmetic_operation(const token_type_t operation) {
    if(operation != MUL && operation != SUB && operation != ADD) { // Check for op type error.
        return;
    }
    char * execute, * convert_rhs;
    switch(operation) { // Set correct labels.
        case MUL: G("LABEL &mul_op"); execute = "?mul_op?execute"; convert_rhs = "?mul_op?convert_rhs"; break;
        case SUB: G("LABEL &sub_op"); execute = "?sub_op?execute"; convert_rhs = "?sub_op?convert_rhs"; break;
        case ADD: G("LABEL &add_op"); execute = "?add_op?execute"; convert_rhs = "?add_op?convert_rhs"; break;
        default: break;
    }
    /* Pop both operands and do basic type checks. */
    pop_operand_arithmetic(lhs, tlhs);
    pop_operand_arithmetic(rhs, trhs);
    /* Type checking for float/int compatibility. */
    G("JUMPIFEQ %s %s %s", execute, tlhs, trhs);
    G("JUMPIFEQ %s %s string@float", convert_rhs, tlhs);
    G("INT2FLOAT %s %s", lhs, lhs);
    G("JUMP %s", execute);
    G("LABEL %s", convert_rhs);
    G("INT2FLOAT %s %s", rhs, rhs);
    G("LABEL %s", execute);
    /* Execution based on op type. */
    switch(operation) {
        case MUL: G("MUL %s %s %s", lhs, lhs, rhs); break;
        case SUB: G("SUB %s %s %s", lhs, lhs, rhs); break;
        case ADD: G("ADD %s %s %s", lhs, lhs, rhs); break;
        default: break;
    }
    G("PUSHS %s", lhs); // Result on top of stack.
    G("RETURN");
    G("\n\n");
} // gen_arithmetic_operation, OK
 
void pop_operand_arithmetic(const char * operand, const char * operand_type) {
        G("POPS %s", operand); // Pop operand to global variable.
        char * type_ok;
        char * skip_nil_conv;
        if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
            error = INTERNAL_ERROR;
            return;
        }
        G("TYPE %s %s", operand_type, operand);
        G("JUMPIFNEQ %s %s string@string", type_ok, operand_type);
        // G("WRITE string@RUNTIME\\032ERROR\\0327:\\032Incompatible\\032operand\\032types!\\010Exiting\\032program!\\010"); // DEBUGGING ONLY!
        G("EXIT int@7");
        G("LABEL %s", type_ok);
        G("JUMPIFNEQ %s %s string@nil", skip_nil_conv, operand_type);
        G("MOVE %s int@0", operand); // nil to int conversion
        G("LABEL %s", skip_nil_conv);
        G("TYPE %s %s", operand_type, operand); // Get type again.
        free(type_ok);
        free(skip_nil_conv);
} // pop_operand_arithmetic, OK

void gen_division_operation() {
    G("LABEL &div_op");
    /* Pop both operands and do type checks and conversions. */
    pop_operand_division(lhs, tlhs);
    pop_operand_division(rhs, trhs);
    /* Division is automatically FLOAT X FLOAT, so we can execute operation immediatly. */
    G("DIV %s %s %s", lhs, lhs, rhs);
    G("PUSHS %s", lhs);
    G("RETURN");
} // gen_division_operation, OK

void pop_operand_division(const char * operand, const char * operand_type) {
    G("POPS %s", operand); // Pop operand to global variable.
    char * type_ok;
    char * skip_nil_conv;
    if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
        error = INTERNAL_ERROR;
        return;
    }
    char * skip_int_conversion = gen_label(NULL, NULL, "?skip_int_conversion", true);
    G("TYPE %s %s", operand_type, operand);
    /* Exit with error if a string type. */
    G("JUMPIFNEQ %s %s string@string", type_ok, operand_type);
    // G("WRITE string@RUNTIME\\032ERROR\\0327:\\032Incompatible\\032operand\\032types!\\010Exiting\\032program!\\010"); // DEBUGGING ONLY!
    G("EXIT int@7");
    G("LABEL %s", type_ok);
    /* Convert to float from nil or int. */
    G("JUMPIFNEQ %s %s string@nil", skip_nil_conv, operand_type);
    G("MOVE %s int@0", operand);
    G("LABEL %s", skip_nil_conv);
    G("TYPE %s %s", operand_type, operand);
    G("JUMPIFEQ %s %s string@float", skip_int_conversion, operand_type);
    G("INT2FLOAT %s %s", operand, operand);
    G("LABEL %s", skip_int_conversion);
    free(skip_int_conversion);
    free(type_ok);
    free(skip_nil_conv);
} // pop_operand_division, OK

void gen_string_operation() {
    G("LABEL &concat_op");
    /* Pop both operands and check types. */
    pop_operand_string(lhs, tlhs);
    pop_operand_string(rhs, trhs);
    /* Concatenation is only STRING X STRING, so type checking is done during pop, we can execute. */
    G("CONCAT %s %s %s", lhs, lhs, rhs);
    G("PUSHS %s", lhs);
    G("RETURN");
    G("\n\n");
} // gen_string_operation, OK

void pop_operand_string(const char * operand, const char * operand_type) {
    G("POPS %s", operand); // Pop operand to global variable.
    char * type_ok;
    char * skip_nil_conv;
    if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
        error = INTERNAL_ERROR;
        return;
    }
    G("TYPE %s %s", operand_type, operand);
    /* Exit with error if operand is not nil or a string. */
    G("JUMPIFEQ %s %s string@string", type_ok, operand_type);
    G("JUMPIFEQ %s %s string@nil", type_ok, operand_type);
    // G("WRITE string@RUNTIME\\032ERROR\\0327:\\032Incompatible\\032operand\\032types!\\010Exiting\\032program!\\010"); // DEBUGGING ONLY!
    G("EXIT int@7");
    G("LABEL %s", type_ok);
    G("JUMPIFNEQ %s %s string@nil", skip_nil_conv, operand_type);
    /* Set operand as empty string if nil. */
    G("MOVE %s string@", operand);
    G("LABEL %s", skip_nil_conv);
    G("TYPE %s %s", operand_type, operand);
    free(type_ok);
    free(skip_nil_conv);
} // pop_operand_string, OK
 
void gen_equality_operation(const token_type_t operation) {
    if(operation != EQ && operation != NEQ) {
        return;
    }
    char * execute, * end;
    switch(operation) { // Set correct labels based on op type.
        case EQ: G("LABEL &eq_op"); execute = "?eq_op?execute"; end = "?eq_op?end"; break;
        case NEQ: G("LABEL &neq_op"); execute = "?neq_op?execute"; end = "?neq_op?end"; break;
        default: break;
    }
    /* Pop operands and do basic type checks. */
    pop_operand_equality(lhs, tlhs);
    pop_operand_equality(rhs, trhs);
    /* If types are equal we can compare directly. */
    G("JUMPIFEQ %s %s %s", execute, tlhs, trhs);
    switch(operation) { // If types not equal we can finish operation with correct return (based on op type).
        case EQ: G("MOVE %s bool@false", lhs); break;
        case NEQ: G("MOVE %s bool@true", lhs); break;
        default: break;
    }
    G("JUMP %s", end);
    G("LABEL %s", execute); // Compare.
    G("EQ %s %s %s", lhs, lhs, rhs);
    if(operation == NEQ) {
        G("NOT %s %s", lhs, lhs); // If neq operation, negate result of EQ operation.
    }
    G("LABEL %s", end);
    G("PUSHS %s", lhs);
    G("RETURN");
    G("\n\n");
} // gen_equality_operation, OK
 
void pop_operand_equality(const char * operand, const char * operand_type) {
    G("POPS %s", operand); // Pop operand to global variable.
    char * type_ok;
    char * skip_nil_conv;
    if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
        error = INTERNAL_ERROR;
        return;
    }
    G("TYPE %s %s", operand_type, operand);
    /* If the operand is of type bool, exit with error. */
    G("JUMPIFNEQ %s %s string@bool", type_ok, operand_type);
    // G("WRITE string@RUNTIME\\032ERROR\\0327:\\032Incompatible\\032operand\\032types!\\010Exiting\\032program!\\010"); // DEBUGGING ONLY!
    G("EXIT int@7");
    G("LABEL %s", type_ok);
    free(type_ok);
    free(skip_nil_conv);
} // pop_operand_equality, OK

void gen_ltgt_operation(token_type_t operation) {
    if(operation != LT && operation != GT) {
        return; // Set error.
    }
    
    
    /* Labels used */
    char * execute = "?lt_op?execute",
        * end = "?lt_op?end",
        * skip_lhs_cast = "?lt_op?skip_lhs_cast",
        * skip_rhs_cast = "?lt_op?skip_rhs_cast",
        * lhs_not_string = "?lt_op?lhs_not_string",
        * rhs_not_string = "?lt_op?rhs_not_string",
        * lhs_not_nil = "?lt_op?lhs_not_nil",
        * rhs_not_nil = "?lt_op?rhs_not_nil";
    switch(operation) { // Set labels based on op type.
        case LT: 
                G("LABEL &lt_op");
                break;
        case GT:
                G("LABEL &gt_op");
                execute = "?gt_op?execute";
                end = "?gt_op?end";
                skip_lhs_cast = "?gt_op?skip_lhs_cast";
                skip_rhs_cast = "?gt_op?skip_rhs_cast";
                lhs_not_string = "?gt_op?lhs_not_string";
                rhs_not_string = "?gt_op?rhs_not_string";
                lhs_not_nil = "?gt_op?lhs_not_nil";
                rhs_not_nil = "?gt_op?rhs_not_nil";
                break;
        default: break;
    }
    /* Pop both operands a do basic type checks. */
    pop_operand_ltgt(lhs, tlhs);
    pop_operand_ltgt(rhs, trhs);
    


    G("JUMPIFNEQ %s %s string@nil", lhs_not_nil, tlhs); // If operand is nil, result is false.
    G("MOVE %s bool@false", lhs);
    G("JUMP %s", end);
    G("LABEL %s", lhs_not_nil);

    G("JUMPIFNEQ %s %s string@nil", rhs_not_nil, trhs); // If operand is nil, result is false.
    G("MOVE %s bool@false", lhs);
    G("JUMP %s", end);
    G("LABEL %s", rhs_not_nil);


    G("JUMPIFEQ %s %s %s", execute, tlhs, trhs); // Types are equal, go to direct comparison.
    /* Check if the comparison is between a float/int and a string, in that case the comparison is false. */
    G("JUMPIFNEQ %s %s string@string", lhs_not_string, tlhs);
    G("MOVE %s bool@false", lhs);
    G("JUMP %s", end);
    G("LABEL %s", lhs_not_string);
    G("JUMPIFNEQ %s %s string@string", rhs_not_string, trhs);
    G("MOVE %s bool@false", lhs);
    G("JUMP %s", end);
    G("LABEL %s", rhs_not_string);

    /* Type cast int to float */
    G("JUMPIFNEQ %s %s string@int", skip_lhs_cast,tlhs); // No type cast.
    G("INT2FLOAT %s %s", lhs, lhs);
    G("LABEL %s", skip_lhs_cast);
    G("JUMPIFNEQ %s %s string@int", skip_rhs_cast, trhs);
    G("INT2FLOAT %s %s", rhs, rhs);
    G("LABEL %s", skip_rhs_cast);

    G("LABEL %s", execute);
    switch(operation) { // Choose instruction based on op type. 
        case LT: G("LT %s %s %s", lhs, lhs, rhs); break;
        case GT: G("GT %s %s %s", lhs, lhs, rhs); break;
        default: break;
    }
    G("LABEL %s", end);
    G("PUSHS %s", lhs);
    G("RETURN");
    G("\n\n");
}

void pop_operand_ltgt(const char * operand, const char * operand_type) {
    G("POPS %s", operand); // Pop operand to global variable.
    char * type_ok;
    char * skip_nil_conv;
    if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
        error = INTERNAL_ERROR;
        return;
    }
    G("TYPE %s %s", operand_type, operand);
    /* If the operand is of type bool, exit with error. */
    G("JUMPIFNEQ %s %s string@bool", type_ok, operand_type);
    // G("WRITE string@RUNTIME\\032ERROR\\0327:\\032Incompatible\\032operand\\032types!\\010Exiting\\032program!\\010"); // DEBUGGING ONLY!
    G("EXIT int@7");
    G("LABEL %s", type_ok);
    free(type_ok);
    free(skip_nil_conv);
} // pop_operand_ltgt, OK

void gen_ltegte_operation(token_type_t operation) {
    if(operation != LTE && operation != GTE) {
        return; // Set errror.
    }
    
    /* Set correct labels. */
    char * execute = "?lte_op?execute",
    * skip_lhs_nil_conversion = "?lte_op?skip_lhs_nil_conversion",
    * skip_rhs_nil_conversion = "?lte_op?skip_rhs_nil_conversion",
    * skip_lhs_to_int_conversion = "?lte_op?skip_lhs_to_int_conversion",
    * skip_rhs_to_int_conversion = "?lte_op?skip_rhs_to_int_conversion",
    * skip_lhs_to_string_conversion = "?lte_op?skip_lhs_to_string_conversion",
    * skip_rhs_to_string_conversion = "?lte_op?skip_rhs_to_string_conversion",
    * lhs_not_string = "?lte_op?lhs_not_string",
    * rhs_not_string = "?lte_op?rhs_not_string",
    * skip_lhs_cast = "?lte_op?skip_lhs_cast",
    * skip_rhs_cast = "?lte_op?skip_rhs_cast",
    * end = "?lte_op?end"; 
    switch(operation) { // Set labels based on op type.
        case LTE: 
                G("LABEL &lte_op");
                break;
        case GTE: 
                G("LABEL &gte_op");
                execute = "?gte_op?execute";
                skip_lhs_nil_conversion = "?gte_op?skip_lhs_nil_conversion";
                skip_rhs_nil_conversion = "?gte_op?skip_rhs_nil_conversion";
                skip_lhs_to_int_conversion = "?gte_op?skip_lhs_to_int_conversion";
                skip_rhs_to_int_conversion = "?gte_op?skip_rhs_to_int_conversion";
                skip_lhs_to_string_conversion = "?gte_op?skip_lhs_to_string_conversion";
                skip_rhs_to_string_conversion = "?gte_op?skip_rhs_to_string_conversion";
                lhs_not_string = "?gte_op?lhs_not_string";
                rhs_not_string = "?gte_op?rhs_not_string";
                skip_lhs_cast = "?gte_op?skip_lhs_cast";
                skip_rhs_cast = "?gte_op?skip_rhs_cast";
                end = "?gte_op?end"; 
                break;
        default: break;
    }
    /* Pop both operands and do basic type checks. */
    pop_operand_ltegte(lhs, tlhs);
    pop_operand_ltegte(rhs, trhs);

    /* Conversion of nil left hand side to correct right hand side type. */
    G("JUMPIFNEQ %s %s string@nil", skip_lhs_nil_conversion, tlhs);
    G("JUMPIFEQ %s %s string@string", skip_lhs_to_int_conversion, trhs);
    G("JUMPIFEQ %s %s string@float", skip_lhs_to_int_conversion, trhs);
    G("MOVE %s int@0", lhs);
    G("TYPE %s %s", tlhs, lhs);
    G("JUMP %s", skip_lhs_nil_conversion);
    G("LABEL %s", skip_lhs_to_int_conversion);
    G("JUMPIFNEQ %s %s string@string", skip_lhs_to_string_conversion, trhs);
    G("MOVE %s string@", lhs);
    G("TYPE %s %s", tlhs, lhs);
    G("JUMP %s", skip_lhs_nil_conversion);
    G("LABEL %s", skip_lhs_to_string_conversion);
    G("MOVE %s int@0", lhs);
    G("INT2FLOAT %s %s", lhs, lhs);
    G("TYPE %s %s", tlhs, lhs);
    G("LABEL %s", skip_lhs_nil_conversion); // Skip if not nil.

    /* Conversion of nil right hand side to correct left hand side type. */
    G("JUMPIFNEQ %s %s string@nil", skip_rhs_nil_conversion, trhs);
    G("JUMPIFEQ %s %s string@string", skip_rhs_to_int_conversion, tlhs);
    G("JUMPIFEQ %s %s string@float", skip_rhs_to_int_conversion, tlhs);
    G("MOVE %s int@0", rhs);
    G("TYPE %s %s", trhs, rhs);
    G("JUMP %s", skip_rhs_nil_conversion);
    G("LABEL %s", skip_rhs_to_int_conversion);
    G("JUMPIFNEQ %s %s string@string", skip_rhs_to_string_conversion, tlhs);
    G("MOVE %s string@", rhs);
    G("TYPE %s %s", trhs, rhs);
    G("JUMP %s", skip_rhs_nil_conversion);
    G("LABEL %s", skip_rhs_to_string_conversion);
    G("MOVE %s int@0", rhs);
    G("INT2FLOAT %s %s", rhs, rhs);
    G("TYPE %s %s", trhs, rhs);
    G("LABEL %s", skip_rhs_nil_conversion); // Skip if not nil.


    G("JUMPIFEQ %s %s %s", execute, tlhs, trhs); // Direct comparison when types are equal..
    /* Check if either type is string. If types not equal and one is string, return as false. */
    G("JUMPIFNEQ %s %s string@string", lhs_not_string, tlhs);
    G("MOVE %s bool@false", lhs);
    G("JUMP %s", end);
    G("LABEL %s", lhs_not_string);
    G("JUMPIFNEQ %s %s string@string", rhs_not_string, trhs);
    G("MOVE %s bool@false", lhs);
    G("JUMP %s", end);
    G("LABEL %s", rhs_not_string);

    /* Float-int type casting. */
    G("JUMPIFNEQ %s %s string@int", skip_lhs_cast,tlhs); // No type cast.
    G("INT2FLOAT %s %s", lhs, lhs);
    G("LABEL %s", skip_lhs_cast);
    G("JUMPIFNEQ %s %s string@int", skip_rhs_cast, trhs);
    G("INT2FLOAT %s %s", rhs, rhs);
    G("LABEL %s", skip_rhs_cast);


    /* EXECUTE */
    G("LABEL %s", execute);
    switch(operation) { // Instruction based on type.
        case LTE: G("LT %s %s %s", aux1, lhs, rhs); break;
        case GTE: G("GT %s %s %s", aux1, lhs, rhs); break;
        default: break;
    }
    G("EQ %s %s %s", lhs, lhs, rhs); // Equality comparison (type safety is insured.)
    G("OR %s %s %s", lhs, lhs, aux1); // Bool || Bool
    G("LABEL %s", end);
    G("PUSHS %s", lhs); // Result returned to stack.
    G("RETURN");
    G("\n\n");
}

void pop_operand_ltegte(const char * operand, const char * operand_type) {
    G("POPS %s", operand); // Pop operand to global variable.
    char * type_ok;
    char * skip_nil_conv;
    if(!gen_pop_labels(&type_ok, &skip_nil_conv)) {
        error = INTERNAL_ERROR;
        return;
    }
    G("TYPE %s %s", operand_type, operand);
    /* If the operand is of type bool, exit with error. */
    G("JUMPIFNEQ %s %s string@bool", type_ok, operand_type);
    // G("WRITE string@RUNTIME\\032ERROR\\0327:\\032Incompatible\\032operand\\032types!\\010Exiting\\032program!\\010"); // DEBUGGING ONLY!
    G("EXIT int@7");
    G("LABEL %s", type_ok);
    free(type_ok);
    free(skip_nil_conv);
}

/* Helper function */
// token_array_t * get_exp(char* expression){
// 	FILE* fp = fopen("./tests/expgen/test_in.php","w+");
// 	if(fp == NULL) exit(1);
// 	fprintf(fp, "%s", expression);
// 	fclose(fp);

// 	freopen("./tests/expgen/test_in.php", "r", stdin);
// 	token_t* t;
// 	token_array_t* ta = token_array_create();
// 	if(ta == NULL){
// 		printf("Allocation error\n");
// 		exit(1);
// 	} 
	
// 	error = OK;
//     while((t = get_token()) != NULL && t->type != END){
// 		if(token_array_push_token(ta, t) != 0){
// 			printf("Allocation error\n");
// 			exit(1);
// 		} 
// 	}
// 	if(t != NULL) t_dstr(t);
	
// 	return parse_expression(ta);
// }
