#ifndef PRECEDENCE_TABLE_H
#define PRECEDENCE_TABLE_H

/**
 * @brief Precedence table index type.
 * 
 */
 typedef enum {
	ADD_SUB_CONC_INDEX,	// +,-,.
	MUL_DIV_INDEX,		// *,/
	IDENTITY_INDEX,		// ===,!==
	REL_INDEX,			// rel
	LEFT_BR_INDEX,		// (
	RIGHT_BR_INDEX,		// )
	LIT_INDEX,			// i
	DOLLAR_INDEX		// $
} prec_table_index_t;

/**
 * @brief Precedence table operation type.
 * 
 */
typedef enum {
	S, // < (Shift)
	R, // > (Reduce)
	E, // = (Equal)
	X  // # (ERR)
} prec_parser_operation_t;

#define PRECEDENCE_TABLE_SIZE 9

/**
 * @brief Precedence table.
 * 
 */
extern const prec_parser_operation_t precedence_table[PRECEDENCE_TABLE_SIZE][PRECEDENCE_TABLE_SIZE];

#endif //PRECEDENCE_TABLE_H
