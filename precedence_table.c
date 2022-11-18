#include "precedence_table.h"
#include <stdlib.h>

/**
 * @brief Precedence table.
 * 
 */
const prec_parser_operation_t precedence_table[PRECEDENCE_TABLE_SIZE][PRECEDENCE_TABLE_SIZE] = {
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

prec_table_index_t get_prec_table_index_of_token(token_t* token){
	if(token == NULL) return DOLLAR_INDEX;
	switch (token->type) {
		case ADD: case SUB: case CONCAT:
			return ADD_SUB_CONC_INDEX;
		case MUL: case DIV:
			return MUL_DIV_INDEX;
		case LT: case GT: case LTE: case GTE:
			return REL_INDEX;
		case EQ: case NEQ:
			return IDENTITY_INDEX;
		case LB:
			return LEFT_BR_INDEX;
		case RB:
			return RIGHT_BR_INDEX;
		case STR_LIT: case INT_LIT:
		case FLT_LIT: case NULL_LIT:
		case VAR_ID:
			return OPERAND_INDEX;
		default:
			return ERROR_INDEX;
	}
}