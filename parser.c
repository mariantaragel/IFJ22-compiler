/**
 * @name parser.c
 * @brief Implementation of top-down parser
 * @authors Marián Tarageľ
 * @date 30.10.2022
 */

#include "parser.h"
#include "scanner.h"
#include "error.h"
#include <stdio.h>
#include <string.h>

#define RETURN_IF_ERROR if (error != OK) return
#define RETURN_ERROR(error_code) error = error_code; return
#define RETURN_ROOT if (error != OK) return root
#define RETURN_INTERNAL_ERROR(node) if (node == NULL) {RETURN_ERROR(INTERNAL_ERROR);}

AST_node_t *program()
{
    AST_node_t *root = AST_create_node(PROG_N);
    if (root == NULL) {
        error = INTERNAL_ERROR;
        return root;
    }
    
    token_t *token = get_token();
    RETURN_ROOT;
    php_start(token);
    //t_dstr(token);
    RETURN_ROOT;

    token = get_token();
    RETURN_ROOT;
    AST_node_t *body = AST_create_add_child(root, BODY_N);
    if (body == NULL) {
        error = INTERNAL_ERROR;
        return root;
    }
    program_body(token, body);
    t_dstr(token);
    RETURN_ROOT;

    return root;
}

void php_start(token_t *token)
{
    // TODO: verify tokens '<?php' func_id '(' 'strict_types' '=' integer_literal ')' ';'

    //if (token->type != PROLOG) {
    //    RETURN_ERROR(SYNTAX_ERROR);
    //}

    for (int i = 0; i < 7; i++) {
        if(i != 0) { //TODO: Delete condition
            token = get_token();
            if (error != OK && i < 7) {
                RETURN_ERROR(SYNTAX_ERROR);
            }
            RETURN_IF_ERROR;
        }

        switch (i)
        {
        case 0:
            if (token->type != FUNC_ID || strcmp(token->sval, "declare")) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 1:
            if (token->type != LB) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 2:
            if (token->type != FUNC_ID || strcmp(token->sval, "strict_types")) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 3:
            if (token->type != ASSIGN) {
                t_dstr(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 4:
            if (token->type != INT_LIT || token->ival != 1) {
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
        RETURN_IF_ERROR;
    }
    
    error = OK;
    stmt(token, parent);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token, parent);
        t_dstr(token);
        RETURN_IF_ERROR;
    }

    error = OK;
    stmt_list_bracket_start(token, parent);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token, parent);
        t_dstr(token);
        RETURN_IF_ERROR;
    }

    error = OK;
    php_end(token);
    RETURN_IF_ERROR;
}

void php_end(token_t *token)
{
    // TODO: verify '?>' EOF | EOF
    if (token->type == END) {
        return;
    }
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
    t_dstr(token);
    n_func_def->data.str = token->sval;
    
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

    type(token, parent);
    RETURN_IF_ERROR;
}

void type(token_t *token, AST_node_t *parent)
{
    AST_node_t *n_type = AST_create_add_child(parent, TYPE_N);
    RETURN_INTERNAL_ERROR(n_type)
    if (token->type != INT_T && token->type != NINT_T && token->type != FLT_T && token->type != NFLT_T && token->type != STR_T && token->type != NSTR_T) {
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
    n_id->data.str = token->sval;
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
    n_id->data.str = token->sval;
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
        // TODO: verify <exp>
        // TODO: add expression to node

        if (token->type != SCOLON) {
            t_dstr(token);
            RETURN_ERROR(SYNTAX_ERROR);
        }
        t_dstr(token);
        break;
    
    default:
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
    }
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
    // TODO: verify <exp>
    // TODO: add expression to node
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != RB) {
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
    // TODO: verify <exp>
    // TODO: add expression to node
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != RB) {
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
    AST_node_t *n_id = AST_create_add_child(n_func_call, ID_N);
    RETURN_INTERNAL_ERROR(n_id)
    n_id->data.str = token->sval;

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
    AST_node_t *n_arg_list = AST_create_add_child(parent, ARG_LIST_N);
    RETURN_INTERNAL_ERROR(n_arg_list)

    if (token->type == RB) {
        return;
    }

    arg(token, n_arg_list);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    arg_next(token, n_arg_list);
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
    if (token->type != VAR_ID && token->type != STR_LIT && token->type != INT_LIT && token->type != FLT_LIT && token->type != NULL_T) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    AST_node_t *n_arg = AST_create_add_child(parent, ARG_N);
    RETURN_INTERNAL_ERROR(n_arg)
    n_arg->data.type = token->type;
}
