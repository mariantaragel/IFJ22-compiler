/**
 * @file precedence_rules.h
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Interface to precedence rules table and rule matching.
 * @date 2022-11-26
 * 
 */

#ifndef PRECEDENCE_RULES_H
#define PRECEDENCE_RULES_H

#include <stdbool.h> // bool
#include "token.h" 

/**
 * @brief Category of precedence rule element.
 * 
 */
typedef enum{
	TERM,	// terminal
	EXPR, 	// expression
	NAN = -1// nothing
}prec_rule_elem_category_t;

/**
 * @brief Element of precedence rule.
 * Element category can be either:
 * 		nonterminal (EXPR),
 * 		terminal (TERM),
 * 		nothing  (NAN).
 * If component is EXPR or NAN, token type should have invalid value -1.
 * If element is TERM token type should have valid value.
 * 
 */
typedef struct prec_rule_elem{
	prec_rule_elem_category_t category;	// category of rule component
	token_type_t token_type;			// type of token
}prec_rule_elem_t;



/**
 * @brief NAN precedence rule element.
 * 
 */
#define PREC_RULE_ELEM_NAN ((prec_rule_elem_t){NAN, -1})

/**
 * @brief EXPR precedence rule element.
 * 
 */
#define PREC_RULE_ELEM_EXPR ((prec_rule_elem_t){EXPR, -1})

/**
 * @brief TERM precedence rule element.
 * 
 */
#define PREC_RULE_ELEM_TERM(token_type) ((prec_rule_elem_t){TERM, token_type})



/**
 * @brief Type of action linked to precedence rule.
 * 
 */
typedef enum {ERROR_ACTION = -1, CONVERT_TO_EXPR , REMOVE_BRACKETS, POSTFIX_MERGE} prec_rule_action_t;


/**
 * @brief Matches rule consiting of three rule elements to associated action.
 * 
 * @param e1 Rule element 1.
 * @param e2 Rule element 2.
 * @param e3 Rule element 3.
 * @return prec_rule_action_t Type of action associated to matched rule. If no rule got matched ERROR_ACTION is returned.
 */
prec_rule_action_t match_precedence_rule(prec_rule_elem_t e1, prec_rule_elem_t e2, prec_rule_elem_t e3);

#endif