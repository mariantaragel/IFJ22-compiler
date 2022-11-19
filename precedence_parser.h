#ifndef PRECEDENCE_PARSER_H
#define PRECEDENCE_PARSER_H

#include "token_array.h"

/**
 * @brief Parses expression stored in token array using precedence bottom-up analysis and returns posfix representation of expression.
 * 
 * If syntax error occured, global error variable is set to SYNTAX_ERROR and NULL is returned.
 * If internal error occured, global error variable is set to INTERNAL_ERROR and NULL is returned.
 * If no internal error occured and syntax analysis was successful, global error variable is set to OK
 * and postfix representation of input expression token array is returned. Returned token array is not NULL.
 * 
 * Warning: Original expression token array is freed. Dont use it after executing this function.
 * 
 * @param expr_token_array Token array representation of expression.
 * @return token_array_t* Returns postfix represenation of expr_token_array on success, otherwise if error occured NULL is returned.
 */
token_array_t* parse_expression(token_array_t *expr_token_array);


#endif
