/**
 * @name parser.c
 * @brief Implementation of top-down parser
 * @authors Marián Tarageľ
 * @date 20.10.2022
 */

#include "parser.h"
#include "scanner.h"
#include "error.h"
#include <stdio.h>

#define RETURN_IF_ERROR if (error != OK) return

void program()
{
    token_t *token = get_token();
    RETURN_IF_ERROR;

    php_start(token);
    RETURN_IF_ERROR;

    token = get_token();
    RETURN_IF_ERROR;

    program_body(token);
    RETURN_IF_ERROR;
}

void php_start(token_t *token)
{
    // TODO: verify tokens '<?php' func_id '(' 'strict_types' '=' integer_literal ')' ';'
    
    if (token->type != PROLOG) {
        error = SYNTAX_ERROR;
        return;
    }
    
    token = get_token();
    if (error != OK) {
        return error;
    }

    return OK;
}

void program_body(token_t *token)
{
    if (!func_def(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!program_body(token)) {
            return OK;
        }
    }
    
    if (!stmt_list(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!program_body(token)) {
            return OK;
        }
    }

    if (!php_end(token)) {
        return OK;
    }

    return SYNTAX_ERROR;
}

void php_end(token_t *token)
{
    // TODO: verify '?>' EOF | EOF
    token = NULL;
    return OK;
}

void func_def(token_t *token)
{
    if (token->type != FUNCTION) {
        return SYNTAX_ERROR;
    }
    
    for (int i = 0; i < 2; i++) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        
        switch (i) {
        case 0:
            if (token->type != FUNC_ID) {
                return SYNTAX_ERROR;
            } break;
        case 1:
            if (token->type != LB) {
                return SYNTAX_ERROR;
            } break;
        default: break;
        }
    }

    token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (param_list(token)) {
        return SYNTAX_ERROR;
    }

    token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (token->type != COLON) {
        return SYNTAX_ERROR;
    }

    token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (return_type(token)) {
        return SYNTAX_ERROR;
    }

    token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (token->type != LCB) {
        return SYNTAX_ERROR;
    }

    token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (stmt_list_bracket_end(token)) {
        return SYNTAX_ERROR;
    }

    return OK;
}

void return_type(token_t *token)
{
    if (!type(token)) {
        return OK;
    }
    if (token->type == VOID_T) {
        return OK;
    }
    return SYNTAX_ERROR;
}

void type(token_t *token)
{
    if (token->type == INT_T || token->type == NINT_T) {
        return OK;
    }
    if (token->type == FLT_T || token->type == NFLT_T) {
        return OK;
    }
    if (token->type == STR_T || token->type == NSTR_T) {
        return OK;
    }
    return SYNTAX_ERROR;
}

void param_list(token_t *token)
{
    if (token->type == RB) {
        return OK;
    }
    if (!type(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (token->type == VAR_ID) {
            token = get_token();
            if (token == NULL)
                return LEXICAL_ERROR;
            if (!param_next(token)) {
                return OK;
            }
        }
    }
    return SYNTAX_ERROR;
}

void param_next(token_t *token)
{
    if (token->type == RB) {
        return OK;
    }
    if (token->type == COMMA) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!type(token)) {
            token = get_token();
            if (token == NULL)
                return LEXICAL_ERROR;
            if (token->type == VAR_ID) {
                token = get_token();
                if (token == NULL)
                    return LEXICAL_ERROR;
                if (!param_next(token)) {
                    return OK;
                }
            }
        }
    }
    return SYNTAX_ERROR;
}

void stmt_list_bracket_end(token_t *token)
{
    if (token->type == RCB) {
        return OK;
    }
    if (!stmt_list(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (token->type == RCB) {
            return OK;
        }
    }
    
    return SYNTAX_ERROR;
}

void stmt_list(token_t *token)
{
    if (token->type == LCB) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!stmt_list_bracket_end(token)) {
            return OK;
        }
    }

    if (!stmt(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;        
        if (!stmt_list(token)) {
            return OK;
        }
    }
    
    return SYNTAX_ERROR;
}

void stmt(token_t *token)
{
    switch (token->type)
    {
    case IF:
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!if_stmt(token)) {
            return OK;
        }
        break;
    
    case WHILE:
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!while_stmt(token)) {
            return OK;
        }
        break;
    
    case RETURN:
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        // TODO: <exp> verification
        if (token->type == SCOLON) {
            return OK;
        }
        break;
    
    default:
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!func_call(token)) {
            token = get_token();
            if (token == NULL)
                return LEXICAL_ERROR;
            if (token->type == SCOLON) {
                return OK;
            }
            
        }
        break;
    }
    
    return SYNTAX_ERROR;
}

void while_stmt(token_t *token)
{
    if (token->type == LB) {
        // TODO: verify <exp>
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (token->type == RB) {
            token = get_token();
            if (token == NULL)
                return LEXICAL_ERROR;
            if (token->type == LCB) {
                token = get_token();
                if (token == NULL)
                    return LEXICAL_ERROR;
                if (!stmt_list_bracket_end(token)) {
                    return OK;
                }
            }
        }
    }

    return SYNTAX_ERROR;
}

void if_stmt(token_t *token)
{
    if (token->type == LB) {
        // TODO: verify <exp>
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (token->type == RB) {
            token = get_token();
            if (token == NULL)
                return LEXICAL_ERROR;
            if (token->type == LCB) {
                token = get_token();
                if (token == NULL)
                    return LEXICAL_ERROR;
                if (!stmt_list_bracket_end(token)) {
                    token = get_token();
                    if (token == NULL)
                        return LEXICAL_ERROR;
                    if (token->type == ELSE) {
                        token = get_token();
                        if (token == NULL)
                            return LEXICAL_ERROR;
                        if (!stmt_list_bracket_end(token)) {
                            return OK;
                        }
                    }
                }
            }
        }
    }

    return SYNTAX_ERROR;
}

void func_call(token_t *token)
{
    token = NULL;
    return OK;
}