/**
 * @name parser.c
 * @brief Implementation of top-down parser
 * @authors Marián Tarageľ
 * @date 16.10.2022
 */

#include "parser.h"
#include "scanner.h"
#include <stdio.h>

int main()
{
    token_t *token = get_token();
    if (token == NULL)
        return LEXICAL_ERROR;
    if (!php_start(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!program_body(token)) {
            printf("great!");
            return SYNTAX_CORRECT;
        }
    }
    printf("error!");
    return SYNTAX_ERROR;
}

int php_start(token_t *token)
{
    // TODO: verify tokens '<?php' func_id '(' 'strict_types' '=' integer_literal ')' ';'
    token = NULL;
    return SYNTAX_CORRECT;
}

int program_body(token_t *token)
{
    if (!func_def(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!program_body(token)) {
            return SYNTAX_CORRECT;
        }
    }
    
    if (!stmt_list(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!program_body(token)) {
            return SYNTAX_CORRECT;
        }
    }

    if (!php_end(token)) {
        return SYNTAX_CORRECT;
    }

    return SYNTAX_ERROR;
}

int php_end(token_t *token)
{
    // TODO: verify '?>' EOF | EOF
    token = NULL;
    return SYNTAX_CORRECT;
}

int func_def(token_t *token)
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

int param_list(token_t *token)
{
    if (token->type == RB) {
        return SYNTAX_CORRECT;
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
                return SYNTAX_CORRECT;
            }
        }
    }
    return SYNTAX_ERROR;
}

int param_next(token_t *token)
{
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
                token = get_token();
                if (token == NULL)
                    return LEXICAL_ERROR;
                if (!param_next(token)) {
                    return SYNTAX_CORRECT;
                }
            }
        }
    }
    return SYNTAX_ERROR;
}

int stmt_list_bracket_end(token_t *token)
{
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
    if (token->type == LCB) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!stmt_list_bracket_end(token)) {
            return SYNTAX_CORRECT;
        }
    }

    if (!stmt(token)) {
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;        
        if (!stmt_list(token)) {
            return SYNTAX_CORRECT;
        }
    }
    
    return SYNTAX_ERROR;
}

int stmt(token_t *token)
{
    switch (token->type)
    {
    case IF:
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!if_stmt(token)) {
            return SYNTAX_CORRECT;
        }
        break;
    
    case WHILE:
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        if (!while_stmt(token)) {
            return SYNTAX_CORRECT;
        }
        break;
    
    case RETURN:
        token = get_token();
        if (token == NULL)
            return LEXICAL_ERROR;
        // TODO: <exp> verification
        if (token->type == SCOLON) {
            return SYNTAX_CORRECT;
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
                return SYNTAX_CORRECT;
            }
            
        }
        break;
    }
    
    return SYNTAX_ERROR;
}

int while_stmt(token_t *token)
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
                    return SYNTAX_CORRECT;
                }
            }
        }
    }

    return SYNTAX_ERROR;
}

int if_stmt(token_t *token)
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
                            return SYNTAX_CORRECT;
                        }
                    }
                }
            }
        }
    }

    return SYNTAX_ERROR;
}

int func_call(token_t *token)
{
    token = NULL;
    return SYNTAX_CORRECT;
}