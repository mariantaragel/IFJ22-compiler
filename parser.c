/**
 * @name parser.c
 * @brief Implementation of top-down parser
 * @authors Marián Tarageľ
 * @date 23.11.2022
 */

#include "parser.h"
#include "scanner.h"
#include "error.h"
#include "precedence_parser.h"
#include "token_array.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define RETURN_IF_ERROR if (error != OK) return
#define RETURN_ERROR(error_code) error = error_code; return
#define RETURN_ROOT if (error != OK) return root
#define RETURN_INTERNAL_ERROR(ptr) if (ptr == NULL) {RETURN_ERROR(INTERNAL_ERROR);}

#define TRUE 1
#define FALSE 0

AST_node_t *program()
{
    // create root node of AST
    AST_node_t *root = AST_create_node(PROG_N);
    if (root == NULL) {
        error = INTERNAL_ERROR;
        return root;
    }
    
    // check prolog of program
    token_t *token = get_token();
    RETURN_ROOT;
    php_start(token);
    t_dstr(token);
    RETURN_ROOT;

    // check main body of program
    token = get_token();
    RETURN_ROOT;
    program_body(token, root);
    t_dstr(token);
    RETURN_ROOT;

    return root;
}

void php_start(token_t *token)
{
    if (token->type != PROLOG) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    for (int i = 0; i < 7; i++) {
        token = get_token();
        RETURN_IF_ERROR;

        switch (i)
        {
        case 0:
            if (token->type != FUNC_ID || strcmp(token->aval, "declare")) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 1:
            if (token->type != LB) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 2:
            if (token->type != FUNC_ID || strcmp(token->aval, "strict_types")) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 3:
            if (token->type != ASSIGN) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 4:
            if (token->type != INT_LIT || strcmp(token->aval, "1")) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 5:
            if (token->type != RB) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 6:
            if (token->type != SCOLON) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        default:
            break;
        }
        t_dstr(token);
    }
}

void program_body(token_t *token, AST_node_t *parent)
{   
    func_def(token, parent);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token, parent);
        t_dstr(token);
        return;
    }
    if (error == LEXICAL_ERROR || error == INTERNAL_ERROR) {
        RETURN_IF_ERROR;
    }
    
    error = OK;
    stmt(token, parent);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token, parent);
        t_dstr(token);
        return;
    }
    if (error == LEXICAL_ERROR || error == INTERNAL_ERROR) {
        RETURN_IF_ERROR;
    }

    error = OK;
    stmt_list_bracket_start(token, parent);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token, parent);
        t_dstr(token);
        return;
    }
    if (error == LEXICAL_ERROR || error == INTERNAL_ERROR) {
        RETURN_IF_ERROR;
    }

    error = OK;
    php_end(token);
    return;
}

void php_end(token_t *token)
{
    if (token->type == END) {
        return;
    }
    else if (token->type != EPILOG) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != END) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);
}

void func_def(token_t *token, AST_node_t *parent)
{
    if (token->type != FUNCTION) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    AST_node_t *n_func_def = AST_create_add_child(parent, FUNC_DEF_N);
    RETURN_INTERNAL_ERROR(n_func_def)
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != FUNC_ID) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    add_aval_to_node(token, n_func_def);
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LB) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    param_list(token, n_func_def);
    t_dstr(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != COLON) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    return_type(token, n_func_def);
    t_dstr(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LCB) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    AST_node_t *n_stmt_list = AST_create_add_child(n_func_def, BODY_N);
    RETURN_INTERNAL_ERROR(n_stmt_list)
    stmt_list_bracket_end(token, n_stmt_list);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void return_type(token_t *token, AST_node_t *parent)
{
    if (token->type == VOID_T) {
        AST_node_t *n_return_type = AST_create_add_child(parent, TYPE_N);
        RETURN_INTERNAL_ERROR(n_return_type)
        n_return_type->data.type = token->type;
        return;
    }
    else {
        type(token, parent);
        RETURN_IF_ERROR;
    }
}

void type(token_t *token, AST_node_t *parent)
{
    AST_node_t *n_type = AST_create_add_child(parent, TYPE_N);
    RETURN_INTERNAL_ERROR(n_type)
    if (!is_token_type_correct(6, token, INT_T, NINT_T, FLT_T, NFLT_T, STR_T, NSTR_T)) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    n_type->data.type = token->type;
}

void param_list(token_t *token, AST_node_t *parent)
{
    AST_node_t *n_param_list = AST_create_add_child(parent, PARAM_LIST_N);
    RETURN_INTERNAL_ERROR(n_param_list)

    if (token->type == RB) {
        return;
    }

    AST_node_t *n_param = AST_create_add_child(n_param_list, PARAM_N);
    RETURN_INTERNAL_ERROR(n_param)
    type(token, n_param);
    RETURN_IF_ERROR;

    AST_node_t *n_id = AST_create_add_child(n_param, ID_N);
    RETURN_INTERNAL_ERROR(n_id)
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != VAR_ID) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    add_aval_to_node(token, n_id);
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    param_next(token, n_param_list);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void param_next(token_t *token, AST_node_t *parent)
{
    if (token->type == RB) {
        return;
    }

    if (token->type != COMMA) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    AST_node_t *n_param = AST_create_add_child(parent, PARAM_N);
    RETURN_INTERNAL_ERROR(n_param)

    token = get_token();
    RETURN_IF_ERROR;
    type(token, n_param);
    t_dstr(token);
    RETURN_IF_ERROR;

    AST_node_t *n_id = AST_create_add_child(n_param, ID_N);
    RETURN_INTERNAL_ERROR(n_id)
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != VAR_ID) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    add_aval_to_node(token, n_id);
    t_dstr(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    param_next(token, parent);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void stmt_list_bracket_end(token_t *token, AST_node_t *parent)
{
    if (token->type == RCB) {
        return;
    }

    stmt(token, parent);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        stmt_list_bracket_end(token, parent);
        t_dstr(token);
        RETURN_IF_ERROR;
    }
    else {
        error = OK;
        stmt_list_bracket_start(token, parent);
        RETURN_IF_ERROR;
        
        token = get_token();
        RETURN_IF_ERROR;
        stmt_list_bracket_end(token, parent);
        t_dstr(token);
        RETURN_IF_ERROR;
    }
}

void stmt_list_bracket_start(token_t *token, AST_node_t *parent)
{
    if (token->type != LCB) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    stmt_list_bracket_end(token, parent);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void stmt(token_t *token, AST_node_t *parent)
{
    switch (token->type)
    {
    case IF:
        token = get_token();
        RETURN_IF_ERROR;
        if_stmt(token, parent);
        t_dstr(token);
        RETURN_IF_ERROR;
        break;
    
    case WHILE:
        token = get_token();
        RETURN_IF_ERROR;
        while_stmt(token, parent);
        t_dstr(token);
        RETURN_IF_ERROR;
        break;
    
    case RETURN:
        token = get_token();
        RETURN_IF_ERROR;
        AST_node_t *n_return = AST_create_add_child(parent, RETURN_N);
        RETURN_INTERNAL_ERROR(n_return)

        AST_node_t *n_expr = AST_create_add_child(n_return, EXPR_N);
        RETURN_INTERNAL_ERROR(n_expr)
        
        token_array_t *array = token_array_create();
        RETURN_INTERNAL_ERROR(array)
        expression(&token, FALSE, array);
        RETURN_IF_ERROR;
        token_array_t *postfix = parse_expression(array);
        RETURN_IF_ERROR;
        n_expr->data.expression = postfix;
        
        if (token->type != SCOLON) {
            t_dstr(token);
            RETURN_ERROR(SYNTAX_ERROR);
        }
        t_dstr(token);
        break;
    
    case FUNC_ID:
        func_call(token, parent);
        RETURN_IF_ERROR;
        token = get_token();
        RETURN_IF_ERROR;
        if (token->type != SCOLON) {
            t_dstr(token);
            RETURN_ERROR(SYNTAX_ERROR);
        }
        t_dstr(token);
        break;
    
    case SCOLON:
        return;
        break;

    default: ;
        if (!is_token_type_correct(18, token, VAR_ID, STR_LIT, INT_LIT, FLT_LIT, MUL,
                        DIV, ADD, SUB, CONCAT, LT, GT, LTE, GTE, EQ, NEQ, LB, RB, NULL_LIT)) {
            RETURN_ERROR(SYNTAX_ERROR);
        }
        token_t *next_token = get_token();
        RETURN_IF_ERROR;
        if (token->type == VAR_ID && next_token->type == ASSIGN) {
            t_dstr(next_token);
            token_t *next_next_token = get_token();
            RETURN_IF_ERROR;
            if (next_next_token->type == FUNC_ID) {
                func_call_assignment(token, parent, next_next_token);
                t_dstr(next_next_token);
                RETURN_IF_ERROR;

                token = get_token();
                RETURN_IF_ERROR;
                if (token->type != SCOLON) {
                    t_dstr(token);
                    RETURN_ERROR(SYNTAX_ERROR);
                }
                t_dstr(token);
            }
            else {
                exp_assignment(token, parent, next_next_token);
                RETURN_IF_ERROR;
            }
        }
        else {
            token_array_t *array = token_array_create();
            RETURN_INTERNAL_ERROR(array)
            token_t *dup_token = t_dup(token);
            RETURN_INTERNAL_ERROR(dup_token)
            if (is_token_type_correct(18, dup_token, VAR_ID, STR_LIT, INT_LIT, FLT_LIT, MUL,
                            DIV, ADD, SUB, CONCAT, LT, GT, LTE, GTE, EQ, NEQ, LB, RB, NULL_LIT)) {
                if (token_array_push_token(array, dup_token)) {
                    t_dstr(token);
                    RETURN_ERROR(INTERNAL_ERROR);
                }
            }
            else {
                token_array_free(array);
                t_dstr(dup_token);
                t_dstr(next_token);
                RETURN_ERROR(SYNTAX_ERROR);
            }
            AST_node_t *n_expr = AST_create_add_child(parent, EXPR_N);
            RETURN_INTERNAL_ERROR(n_expr);

            token = next_token;
            dup_token = t_dup(token);
            RETURN_INTERNAL_ERROR(dup_token)
            expression(&dup_token, FALSE, array);
            RETURN_IF_ERROR;
            token = dup_token;
            token_array_t *postfix = parse_expression(array);
            RETURN_IF_ERROR;
            n_expr->data.expression = postfix;

            if (token->type != SCOLON) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            }
            t_dstr(next_token);
            t_dstr(token);
        }
        
        break;
    }
}

void exp_assignment(token_t *token, AST_node_t *parent, token_t *exp_token)
{
    AST_node_t *n_ass_exp = AST_create_add_child(parent, ASS_EXPR_N);
    RETURN_INTERNAL_ERROR(n_ass_exp);

    AST_node_t *n_id = AST_create_add_child(n_ass_exp, ID_N);
    RETURN_INTERNAL_ERROR(n_id)
    add_aval_to_node(token, n_id);

    AST_node_t *n_expr = AST_create_add_child(n_ass_exp, EXPR_N);
    RETURN_INTERNAL_ERROR(n_expr);

    if (!is_token_type_correct(18, exp_token, VAR_ID, STR_LIT, INT_LIT, FLT_LIT, MUL,
                            DIV, ADD, SUB, CONCAT, LT, GT, LTE, GTE, EQ, NEQ, LB, RB, NULL_LIT)) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token_array_t *array = token_array_create();
    RETURN_INTERNAL_ERROR(array)
    expression(&exp_token, FALSE, array);
    RETURN_IF_ERROR;
    token_array_t *postfix = parse_expression(array);
    RETURN_IF_ERROR;
    n_expr->data.expression = postfix;

    if (exp_token->type != SCOLON) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(exp_token);
}

void func_call_assignment(token_t *token, AST_node_t *parent, token_t *func_id_token)
{
    AST_node_t *n_ass_func = AST_create_add_child(parent,ASS_FUNC_N);
    RETURN_INTERNAL_ERROR(n_ass_func);

    AST_node_t *n_id = AST_create_add_child(n_ass_func, ID_N);
    RETURN_INTERNAL_ERROR(n_id)
    add_aval_to_node(token, n_id);

    func_call(func_id_token, n_ass_func);
    RETURN_IF_ERROR;
}

void while_stmt(token_t *token, AST_node_t *parent)
{
    AST_node_t *n_while_stmt = AST_create_add_child(parent, WHILE_N);
    RETURN_INTERNAL_ERROR(n_while_stmt)
    
    if (token->type != LB) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    
    AST_node_t *n_expr = AST_create_add_child(n_while_stmt, EXPR_N);
    RETURN_INTERNAL_ERROR(n_expr)
    
    token = get_token();
    RETURN_IF_ERROR;
    token_array_t *array = token_array_create();
    RETURN_INTERNAL_ERROR(array)
    expression(&token, TRUE, array);
    RETURN_IF_ERROR;
    token_array_t *postfix = parse_expression(array);
    RETURN_IF_ERROR;
    n_expr->data.expression = postfix;
    
    if (token->type != LCB) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    AST_node_t *n_stmt_list = AST_create_add_child(n_while_stmt, BODY_N);
    RETURN_INTERNAL_ERROR(n_stmt_list)
    stmt_list_bracket_end(token, n_stmt_list);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void if_stmt(token_t *token, AST_node_t *parent)
{
    AST_node_t *n_if_stmt = AST_create_add_child(parent, IF_N);
    RETURN_INTERNAL_ERROR(n_if_stmt)

    if (token->type != LB) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    
    AST_node_t *n_expr = AST_create_add_child(n_if_stmt, EXPR_N);
    RETURN_INTERNAL_ERROR(n_expr)

    token = get_token();
    RETURN_IF_ERROR;
    token_array_t *array = token_array_create();
    RETURN_INTERNAL_ERROR(array)
    expression(&token, TRUE, array);
    RETURN_IF_ERROR;
    token_array_t *postfix = parse_expression(array);
    RETURN_IF_ERROR;
    n_expr->data.expression = postfix;
    
    if (token->type != LCB) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    AST_node_t *n_stmt_list_if = AST_create_add_child(n_if_stmt, BODY_N);
    RETURN_INTERNAL_ERROR(n_stmt_list_if)
    stmt_list_bracket_end(token, n_stmt_list_if);
    t_dstr(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != ELSE) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LCB) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    AST_node_t *n_stmt_list_else = AST_create_add_child(n_if_stmt, BODY_N);
    RETURN_INTERNAL_ERROR(n_stmt_list_else)
    stmt_list_bracket_end(token, n_stmt_list_else);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void func_call(token_t *token, AST_node_t *parent)
{
    if (token->type != FUNC_ID) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    AST_node_t *n_func_call = AST_create_add_child(parent, FUNC_CALL_N);
    RETURN_INTERNAL_ERROR(n_func_call)
    add_aval_to_node(token, n_func_call);

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LB) {
        t_dstr(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    t_dstr(token);

    token = get_token();
    RETURN_IF_ERROR;
    arg_list(token, n_func_call);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void arg_list(token_t *token, AST_node_t *parent)
{
    if (token->type == RB) {
        return;
    }

    arg(token, parent);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    arg_next(token, parent);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void arg_next(token_t *token, AST_node_t *parent)
{
    if (token->type == RB) {
        return;
    }
    if (token->type != COMMA) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    arg(token, parent);
    t_dstr(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    arg_next(token, parent);
    t_dstr(token);
    RETURN_IF_ERROR;
}

void arg(token_t *token, AST_node_t *parent)
{
    if (!is_token_type_correct(5, token, VAR_ID, STR_LIT, INT_LIT, FLT_LIT, NULL_LIT)) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    if (token->type == VAR_ID) {
        AST_node_t *n_arg = AST_create_add_child(parent, ID_N);
        RETURN_INTERNAL_ERROR(n_arg)
        add_aval_to_node(token, n_arg);
    }
    else if (token->type == STR_LIT) {
        AST_node_t *n_arg = AST_create_add_child(parent, STR_LIT_N);
        RETURN_INTERNAL_ERROR(n_arg)
        add_aval_to_node(token, n_arg);
    }
    else if (token->type == INT_LIT) {
        AST_node_t *n_arg = AST_create_add_child(parent, INT_LIT_N);
        RETURN_INTERNAL_ERROR(n_arg)
        add_aval_to_node(token, n_arg);
    }
    else if (token->type == FLT_LIT) {
        AST_node_t *n_arg = AST_create_add_child(parent, FLT_LIT_N);
        RETURN_INTERNAL_ERROR(n_arg)
        add_aval_to_node(token, n_arg);
    }
    else if (token->type == NULL_LIT) {
        AST_node_t *n_arg = AST_create_add_child(parent, NULL_LIT_N);
        RETURN_INTERNAL_ERROR(n_arg)
    }
}

/**
 * @brief Checks if token type is equivalent to one of the parameters
 */
int is_token_type_correct(int num_of_types, token_t *token, ...)
{
    va_list valist;
    va_start(valist, token);

    int return_status = FALSE;
    for (int i = 0; i < num_of_types; i++) {
        if (va_arg(valist, token_type_t) == token->type) {
            return_status = TRUE;
            break;
        }
    }

    va_end(valist);

    return return_status;
}

/**
 * @brief Reads expression to token array
 */
void expression(token_t **token, int is_in_if_or_while, token_array_t *array)
{
    while (is_token_type_correct(18, *token, VAR_ID, STR_LIT, INT_LIT, FLT_LIT, MUL,
                            DIV, ADD, SUB, CONCAT, LT, GT, LTE, GTE, EQ, NEQ, LB, RB, NULL_LIT)) {
        if (token_array_push_token(array, *token)) {
            t_dstr(*token);
            RETURN_ERROR(INTERNAL_ERROR);
        }
        *token = get_token();
        RETURN_IF_ERROR;
    }
    
    if (is_in_if_or_while) {
        if (array->token_count == 0UL) {
            RETURN_ERROR(SYNTAX_ERROR);
        }
        token_t *prev_token = token_array_pop_token(array);
        if (prev_token->type != RB || array->token_count == 0UL) {
            t_dstr(prev_token);
            RETURN_ERROR(SYNTAX_ERROR);
        }
        t_dstr(prev_token);
    }
}

/**
 * @brief Associated value of token will be moved to AST node
 */
void add_aval_to_node(token_t *token, AST_node_t *node)
{
    char *new_value = malloc((strlen(token->aval) + 1) * sizeof(char));
    RETURN_INTERNAL_ERROR(new_value)
    strcpy(new_value, token->aval);
    node->data.str = new_value;
}