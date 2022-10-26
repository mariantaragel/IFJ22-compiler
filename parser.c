/**
 * @name parser.c
 * @brief Implementation of top-down parser
 * @authors Marián Tarageľ
 * @date 21.10.2022
 */

#include "parser.h"
#include "scanner.h"
#include "error.h"
#include <stdio.h>
#include <string.h>

#define RETURN_IF_ERROR if (error != OK) return
#define RETURN_ERROR(error_code) error = error_code; return

int main() {
    program();
    if (error == OK) {
        printf("OK!\n");
    } else if (error == SEM_ERROR_3) {
        printf("OK!\n");
    } else {
        printf("ERROR!\n");
    }
}

void program()
{
    token_t *token = get_token();
    RETURN_IF_ERROR;
    php_start(token);
    //free(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    program_body(token);
    free(token);
    RETURN_IF_ERROR;
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
                free(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 1:
            if (token->type != LB) {
                free(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 2:
            if (token->type != FUNC_ID || strcmp(token->sval, "strict_types")) {
                free(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 3:
            if (token->type != ASSIGN) {
                free(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 4:
            if (token->type != INT_LIT || token->ival != 1) {
                free(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        case 5:
            if (token->type != RB) {
                free(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;
        
        case 6:
            if (token->type != SCOLON) {
                free(token);
                RETURN_ERROR(SYNTAX_ERROR);
            } break;

        default:
            break;
        }
    }
}

void program_body(token_t *token)
{
    func_def(token);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token);
        free(token);
        RETURN_IF_ERROR;
    }
    
    error = OK;
    stmt(token);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token);
        free(token);
        RETURN_IF_ERROR;
    }

    error = OK;
    stmt_list_bracket_start(token);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        program_body(token);
        free(token);
        RETURN_IF_ERROR;
    }

    /*
    error = OK;
    php_end(token);
    RETURN_IF_ERROR;
    */
}

/*
void php_end(token_t *token)
{
    // TODO: verify '?>' EOF | EOF
    if (token->type != EPILOG) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
}
*/

void func_def(token_t *token)
{
    if (token->type != FUNCTION) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != FUNC_ID) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);

    token = get_token();
    RETURN_IF_ERROR;
    param_list(token);
    free(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != COLON) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    return_type(token);
    free(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LCB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);

    token = get_token();
    RETURN_IF_ERROR;
    stmt_list_bracket_end(token);
    free(token);
    RETURN_IF_ERROR;
}

void return_type(token_t *token)
{
    if (token->type == VOID_T) {
        return;
    }

    type(token);
    RETURN_IF_ERROR;
}

void type(token_t *token)
{
    if (token->type != INT_T && token->type != NINT_T && token->type != FLT_T && token->type != NFLT_T && token->type != STR_T && token->type != NSTR_T) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
}

void param_list(token_t *token)
{
    if (token->type == RB) {
        return;
    }

    type(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != VAR_ID) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);

    token = get_token();
    RETURN_IF_ERROR;
    param_next(token);
    free(token);
    RETURN_IF_ERROR;
}

void param_next(token_t *token)
{
    if (token->type == RB) {
        return;
    }

    if (token->type != COMMA) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    type(token);
    free(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != VAR_ID) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    param_next(token);
    free(token);
    RETURN_IF_ERROR;
}

void stmt_list_bracket_end(token_t *token)
{
    if (token->type == RCB) {
        return;
    }

    stmt(token);
    if (error == OK) {
        token = get_token();
        RETURN_IF_ERROR;
        stmt_list_bracket_end(token);
        free(token);
        RETURN_IF_ERROR;
    }
    else {
        error = OK;
        stmt_list_bracket_start(token);
        RETURN_IF_ERROR;
        
        token = get_token();
        RETURN_IF_ERROR;
        stmt_list_bracket_end(token);
        free(token);
        RETURN_IF_ERROR;
    }
}

void stmt_list_bracket_start(token_t *token)
{
    if (token->type != LCB) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    stmt_list_bracket_end(token);
    free(token);
    RETURN_IF_ERROR;
}

void stmt(token_t *token)
{
    switch (token->type)
    {
    case IF:
        token = get_token();
        RETURN_IF_ERROR;
        if_stmt(token);
        free(token);
        RETURN_IF_ERROR;
        break;
    
    case WHILE:
        token = get_token();
        RETURN_IF_ERROR;
        while_stmt(token);
        free(token);
        RETURN_IF_ERROR;
        break;
    
    case RETURN:
        token = get_token();
        RETURN_IF_ERROR;
        // TODO: verify <exp>
        if (token->type != SCOLON) {
            free(token);
            RETURN_ERROR(SYNTAX_ERROR);
        }
        free(token);
        break;
    
    default:
        func_call(token);
        RETURN_IF_ERROR;
        token = get_token();
        RETURN_IF_ERROR;
        if (token->type != SCOLON) {
            free(token);
            RETURN_ERROR(SYNTAX_ERROR);
        }
        free(token);
        break;
    }
}

void while_stmt(token_t *token)
{
    if (token->type != LB) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    // TODO: verify <exp>
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != RB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LCB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    stmt_list_bracket_end(token);
    free(token);
    RETURN_IF_ERROR;
}

void if_stmt(token_t *token)
{
    if (token->type != LB) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
    // TODO: verify <exp>
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != RB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LCB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);
    
    token = get_token();
    RETURN_IF_ERROR;
    stmt_list_bracket_end(token);
    free(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != ELSE) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LCB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);

    token = get_token();
    RETURN_IF_ERROR;
    stmt_list_bracket_end(token);
    free(token);
    RETURN_IF_ERROR;
}

void func_call(token_t *token)
{
    if (token->type != FUNC_ID) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    if (token->type != LB) {
        free(token);
        RETURN_ERROR(SYNTAX_ERROR);
    }
    free(token);

    token = get_token();
    RETURN_IF_ERROR;
    arg_list(token);
    free(token);
    RETURN_IF_ERROR;
}

void arg_list(token_t *token)
{
    if (token->type == RB) {
        return;
    }

    arg(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    arg_next(token);
    free(token);
    RETURN_IF_ERROR;
}

void arg_next(token_t *token)
{
    if (token->type == RB) {
        return;
    }
    if (token->type != COMMA) {
        RETURN_ERROR(SYNTAX_ERROR);
    }

    token = get_token();
    RETURN_IF_ERROR;
    arg(token);
    free(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;
    arg_next(token);
    free(token);
    RETURN_IF_ERROR;
}

void arg(token_t *token)
{
    if (token->type != VAR_ID && token->type != STR_LIT && token->type != INT_LIT && token->type != FLT_LIT && token->type != NULL_T) {
        RETURN_ERROR(SYNTAX_ERROR);
    }
}
