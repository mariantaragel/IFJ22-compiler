#include "precedence_table.h"

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