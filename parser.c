/**
 * @name parser.c
 * @brief Implementation of top-down parser
 * @authors Marián Tarageľ
 * @date 10.10.2022
 */

#include "parser.h"
#include "scanner.h"

int program()
{
    int program = FALSE;
    if (php_start()) {
        if (program_body()) {
            if (php_end()) {
                program = TRUE;
            }
        }
    }
    return program;
}

int php_start()
{
    int php_start = FALSE;
    for (int i = 0; i < 8; i++) {
        token_t *token = get_token();
        // TODO: verify tokens '<?php' func_id '(' 'strict_types' '=' integer_literal ')' ';' (swtich-case?)
        if (php_start == FALSE)
            break;
    }
    return php_start;
}

int program_body()
{
    // TODO
    return 0;
}

int php_end()
{
    // TODO
    return 0;
}