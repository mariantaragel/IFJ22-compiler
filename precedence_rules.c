#include "precedence_rules.h"
#include "token.h"
#include "stdbool.h"


bool is_prec_rule_elem_term(prec_rule_elem_t elem){
	if(elem.category == TERM)
		return true;
	else
		return false;
}

/**
 * @brief Precedence rule structure type.
 * Contains three precedence rule elements and associated action type.
 * 
 */
typedef struct{
	prec_rule_elem_t e1, e2, e3;// elements of rule
	prec_rule_action_t action;	// associated action type
}prec_rule_t;

/**
 * @brief Number of precedence rules.
 */
#define PRECEDENCE_RULE_COUNT 17

/**
 * @brief Table of precedence rules.
 */
const prec_rule_t precedence_rules[PRECEDENCE_RULE_COUNT] = {
	{{TERM, VAR_ID	}, {NAN,  -1	}, {NAN,  -1	}, CONVERT_TO_EXPR	}, //1.  E -> i
	{{TERM, STR_LIT	}, {NAN,  -1	}, {NAN,  -1	}, CONVERT_TO_EXPR	}, //2.  E -> i
	{{TERM, FLT_LIT	}, {NAN,  -1	}, {NAN,  -1	}, CONVERT_TO_EXPR	}, //3.  E -> i
	{{TERM, INT_LIT	}, {NAN,  -1	}, {NAN,  -1	}, CONVERT_TO_EXPR	}, //4.  E -> i
	{{TERM, NULL_T	}, {NAN,  -1	}, {NAN,  -1	}, CONVERT_TO_EXPR	}, //5.  E -> i // TODO: NULL_T -> NULL_LIT
	{{TERM, LB		}, {EXPR, -1	}, {TERM, RB	}, REMOVE_BRACKETS	}, //6.  E -> ( E )
	{{EXPR,	-1		}, {TERM, ADD	}, {EXPR, -1	}, POSTFIX_MERGE	}, //7.  E -> E + E
	{{EXPR,	-1      }, {TERM, SUB	}, {EXPR, -1	}, POSTFIX_MERGE	}, //8.  E -> E - E
	{{EXPR,	-1      }, {TERM, MUL	}, {EXPR, -1	}, POSTFIX_MERGE	}, //9.  E -> E * E
	{{EXPR,	-1		}, {TERM, DIV	}, {EXPR, -1	}, POSTFIX_MERGE	}, //10.  E -> E / E
	{{EXPR,	-1		}, {TERM, CONCAT}, {EXPR, -1	}, POSTFIX_MERGE	}, //11. E -> E . E
	{{EXPR,	-1		}, {TERM, LT	}, {EXPR, -1	}, POSTFIX_MERGE	}, //12. E -> E < E
	{{EXPR,	-1		}, {TERM, GT	}, {EXPR, -1	}, POSTFIX_MERGE	}, //13. E -> E > E
	{{EXPR,	-1		}, {TERM, LTE	}, {EXPR, -1	}, POSTFIX_MERGE	}, //14. E -> E <= E
	{{EXPR,	-1		}, {TERM, GTE	}, {EXPR, -1	}, POSTFIX_MERGE	}, //15. E -> E >= E
	{{EXPR,	-1		}, {TERM, EQ	}, {EXPR, -1	}, POSTFIX_MERGE	}, //16. E -> E === E
	{{EXPR,	-1		}, {TERM, NEQ	}, {EXPR, -1	}, POSTFIX_MERGE	}  //17. E -> E !== E
};


bool match_rule_elem(prec_rule_elem_t x, prec_rule_elem_t y){
	if(x.category == y.category && x.token_type == y.token_type)
		return true;
	else
		return false;
}

prec_rule_action_t match_precedence_rule(prec_rule_elem_t e1, prec_rule_elem_t e2, prec_rule_elem_t e3){
	prec_rule_t cur_rule;
	for(int i = 0; i < PRECEDENCE_RULE_COUNT; ++i){
		cur_rule = precedence_rules[i];
		if(match_rule_elem(cur_rule.e1, e1) && match_rule_elem(cur_rule.e2, e2) && match_rule_elem(cur_rule.e3, e3)){
			return cur_rule.action;
		}
	}

	return ERROR_ACTION;
}
