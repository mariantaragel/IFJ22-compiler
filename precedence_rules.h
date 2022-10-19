#ifndef PRECEDENCE_RULES_H
#define PRECEDENCE_RULES_H

#include "token.h"

/**
 * @brief Category of precedence rule element.
 * 
 */
typedef enum{
	TERM,	// terminal
	EXPR, 	// expression
	NAN		// nothing
}prec_rule_elem_category_t;

/**
 * @brief Element of precedence rule structure type.
 * Element category can be either:
 * 		nonterminal (EXPR),
 * 		terminal (TERM),
 * 		nothing  (NAN).
 * If component is EXPR or NAN, token type has invalid value -1.
 * If element is TERM token type has valid value.
 * 
 */
typedef struct{
	prec_rule_elem_category_t category;	// category of rule component
	token_type_t token_type;			// type of token
}prec_rule_elem_t;

/**
 * @brief Number of precedence rules.
 */
#define PRECEDENCE_RULE_COUNT 16

/**
 * @brief Maximum number of elements in precedence rule.
 */
#define PRECEDENCE_RULE_SIZE 3

/**
 * @brief Table of precedence rules.
 */
const prec_rule_elem_t precedence_rules[PRECEDENCE_RULE_COUNT][PRECEDENCE_RULE_SIZE] = {
	{{TERM, VAR_ID	}, {NAN,  -1	}, {NAN,  -1	}}, //1.  E -> i
	{{TERM, STR_LIT	}, {NAN,  -1	}, {NAN,  -1	}}, //2.  E -> i
	{{TERM, FLT_LIT	}, {NAN,  -1	}, {NAN,  -1	}}, //3.  E -> i
	{{TERM, INT_LIT	}, {NAN,  -1	}, {NAN,  -1	}}, //4.  E -> i
	{{TERM, LB		}, {EXPR, -1	}, {TERM, RB	}}, //5.  E -> ( E )
	{{EXPR,	-1		}, {TERM, ADD	}, {EXPR, -1	}}, //6.  E -> E + E
	{{EXPR,	-1      }, {TERM, SUB	}, {EXPR, -1	}}, //7.  E -> E - E
	{{EXPR,	-1      }, {TERM, MUL	}, {EXPR, -1	}}, //8.  E -> E * E
	{{EXPR,	-1		}, {TERM, DIV	}, {EXPR, -1	}}, //9.  E -> E / E
	{{EXPR,	-1		}, {TERM, CONCAT}, {EXPR, -1	}}, //10. E -> E . E
	{{EXPR,	-1		}, {TERM, LT	}, {EXPR, -1	}}, //11. E -> E < E
	{{EXPR,	-1		}, {TERM, GT	}, {EXPR, -1	}}, //12. E -> E > E
	{{EXPR,	-1		}, {TERM, LTE	}, {EXPR, -1	}}, //13. E -> E <= E
	{{EXPR,	-1		}, {TERM, GTE	}, {EXPR, -1	}}, //14. E -> E >= E
	{{EXPR,	-1		}, {TERM, EQ	}, {EXPR, -1	}}, //15. E -> E === E
	{{EXPR,	-1		}, {TERM, NEQ	}, {EXPR, -1	}}  //16. E -> E !== E
};

#endif