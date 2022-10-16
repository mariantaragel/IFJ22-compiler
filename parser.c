/**
 * @name parser.c
 * @brief Implementation of top-down parser
 * @authors Marián Tarageľ
 * @date 15.10.2022
 */

#include "parser.h"
#include "scanner.h"
#include <stdio.h>

int program()
{
    if (!php_start()) {
        if (!program_body()) {
            return SYNTAX_CORRECT;
        }
    }
    return SYNTAX_ERROR;
}

int php_start()
{
    for (int i = 0; i < 8; i++) {
        token_t *token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        // TODO: verify tokens '<?php' func_id '(' 'strict_types' '=' integer_literal ')' ';'
    }
    return SYNTAX_CORRECT;
}

int program_body()
{
    func_def();
    program_body();

    //stmt_list();
    program_body();

    return SYNTAX_CORRECT;
}

int php_end()
{
    token_t *token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    // TODO: verify '?>'
    return SYNTAX_CORRECT;
}

int func_def()
{
    token_t *token;
    for (int i = 0; i < 3; i++) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        
        switch (i) {
        case 0:
            if (token->type != FUNCTION) {
                return SYNTAX_ERROR;
            } break;
        case 1:
            if (token->type != FUNC_ID) {
                return SYNTAX_ERROR;
            } break;
        case 2:
            if (token->type != LB) {
                return SYNTAX_ERROR;
            } break;
        default: break;
        }
    }

    if (param_list()) {
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

    if (stmt_list_bracket_end()) {
        return SYNTAX_ERROR;
    }

    return SYNTAX_CORRECT;
}

int return_type(token_t *token)
{
    if (!type(token)) {
        return SYNTAX_CORRECT;
    }
    if (token->type == VOID_T) {
        return SYNTAX_CORRECT;
    }
    return SYNTAX_ERROR;
}

int type(token_t *token)
{
    if (token->type == INT_T || token->type == NINT_T) {
        return SYNTAX_CORRECT;
    }
    if (token->type == FLT_T || token->type == NFLT_T) {
        return SYNTAX_CORRECT;
    }
    if (token->type == STR_T || token->type == NSTR_T) {
        return SYNTAX_CORRECT;
    }
    return SYNTAX_ERROR;
}

int param_list()
{
    token_t *token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (token->type == RB) {
        return SYNTAX_CORRECT;
    }
    if (!type(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (token->type == VAR_ID) {
            if (!param_next()) {
                return SYNTAX_CORRECT;
            }
        }
    }
    return SYNTAX_ERROR;
}

int param_next()
{
    token_t *token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (token->type == RB) {
        return SYNTAX_CORRECT;
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
                if (!param_next()) {
                    return SYNTAX_CORRECT;
                }
            }
        }
    }
    return SYNTAX_ERROR;
}

int stmt_list_bracket_end()
{
    token_t *token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (token->type == RCB) {
        return SYNTAX_CORRECT;
    }
    if (!stmt_list(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (token->type == RCB) {
            return SYNTAX_CORRECT;
        }
    }
    
    return SYNTAX_ERROR;
}

int stmt_list(token_t *token)
{
    if (token->type == IF) {
        return SYNTAX_CORRECT;
    }
    return SYNTAX_CORRECT;
}