#ifndef PRECEDENCE_PARSER_H
#define PRECEDENCE_PARSER_H

#include "token_array.h"

/**
 * @brief Parses expression bottom-up. Complements top_down parser.
 * 
 * @param token_array_t* Token array created by top-down parser.
 * @return token_array_t* Returns parsed array to top-down.
 */
token_array_t* parse_expression(token_array_t *expr_token_array);


#endif
