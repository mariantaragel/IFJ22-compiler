#ifndef _PRECEDENCE_H
#define _PRECEDENCE_H

// INDEXES
 typedef enum {
	ADD_SUB_INDEX,		    // +,-
	MUL_DIV_INDEX,		    // *,/
	OP_TYPE_INDEX,		    // ===,!==
	REL_INDEX,		    // rel
	LEFT_BR_INDEX,		    // (
	RIGHT_BR_INDEX,	      	    // )
  CONCATENATION_INDEX,  	    // .
	LIT_INDEX,		    // i
	DOLLAR_INDEX		    // $
} precedence_index;

typedef enum {
	L, // > (Lower)
 	S, // < (Shift)
 	R, // = (Remain)
 	N  // # (ERR)
} precedence_table_sign;

int precedence_table[9][9] = {
    //	        |+,-|*,/|===|rel| ( | ) | . | i | $ |
    /* +,- */   { L , S , L , L , S , L , L , S , L }, 
    /* *,/ */   { L , L , L , L , S , L , L , S , L },
    /* === */   { S , S , L , S , S , L , S , S , L }, 
    /* rel */   { S , S , L , L , S , L , S , S , L }, 
    /* ( */     { S , S , S , S , S , R , S , S , N },
    /* ) */     { L , L , L , L , N , L , L , N , L },
    /* . */     { L , S , L , L , S , L , L , S , L },
    /* i */     { L , L , L , L , N , L , L , N , L },
    /* $ */     { S , S , S , S , S , N , S , S , N }      
};

/*
// FIGURES
typedef enum {
	ADDITION,	 // +
	SUBTRACTION,	 // -
	MULTIPLICATION,	 // *
	DIVISION,	 // /
	EQU,		 // = // ?????
 	RELATION_OP,   	 // rel
	LEFT_BR,	 // (
	RIGHT_BR,	 // )		           
	ITG,    	 // int
	DBL,        	 // double
  	//ID ??
	DOLLAR,		 // $
} precedence_figure;
*/

/*
// EXPRESION PARSING RULES ??
*/

#endif //_PRECEDENCE_H
