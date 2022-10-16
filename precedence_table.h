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

/**
 * @brief Precedence table.
 * 
 */
const prec_parser_operation_t precedence_table[9][9] = {
    //	        |+,-,.| *,/ | === | rel |  (  |  )  |  i  |  $  |
    /* +,-,. */ {  R  ,  S  ,  R  ,  R  ,  S  ,  R  ,  S  ,  R  }, 
    /*  *,/  */ {  R  ,  R  ,  R  ,  R  ,  S  ,  R  ,  S  ,  R  },
    /*  ===  */ {  S  ,  S  ,  X  ,  S  ,  S  ,  R  ,  S  ,  R  }, 
    /*  rel  */ {  S  ,  S  ,  R  ,  X  ,  S  ,  R  ,  S  ,  R  }, 
    /*   (   */ {  S  ,  S  ,  S  ,  S  ,  S  ,  E  ,  S  ,  X  },
    /*   )   */ {  R  ,  R  ,  R  ,  R  ,  X  ,  R  ,  X  ,  R  },
    /*   i   */ {  R  ,  R  ,  R  ,  R  ,  X  ,  R  ,  X  ,  R  },
    /*   $   */ {  S  ,  S  ,  S  ,  S  ,  S  ,  X  ,  S  ,  X  }      
};

#endif //PRECEDENCE_TABLE_H
