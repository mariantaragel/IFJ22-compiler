#ifndef PRECEDENCE_TABLE_H
#define PRECEDENCE_TABLE_H

#include "token.h"

/**
 * @brief Precedence table index type.
 * 
 */
 typedef enum {
	ERROR_INDEX = -1,
	ADD_SUB_CONC_INDEX,	// +,-,.
	MUL_DIV_INDEX,		// *,/
	IDENTITY_INDEX,		// ===,!==
	REL_INDEX,			// rel
	LEFT_BR_INDEX,		// (
	RIGHT_BR_INDEX,		// )
	OPERAND_INDEX,		// i
	DOLLAR_INDEX		// $
} prec_table_index_t;

/**
 * @brief Get the precedence table index of token.
 * 
 * @param token Token to get precedence table index off.
 * @return prec_table_index_t Precedence table index of token.
 */
prec_table_index_t get_prec_table_index_of_token(token_t* token);

/**
 * @brief Precedence parser operation type.
 * 
 */
typedef enum {
	S, // < (Shift)
	R, // > (Reduce)
	E, // = (Equal)
	X  // # (ERR)
} prec_parser_operation_t;

/**
 * @brief Return precedence analyzer operation from precedence table.
 * 
 * @param top_term_index Precedence index of top precedence stack terminal.
 * @param input_term_index Precedence index of input terminal.
 * @return prec_parser_operation_t Precedence parser operation.
 */
prec_parser_operation_t precedence_table_get_operation(prec_table_index_t top_term_index, prec_table_index_t input_term_index);


#endif //PRECEDENCE_TABLE_H
