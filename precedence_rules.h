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
extern const prec_rule_elem_t precedence_rules[PRECEDENCE_RULE_COUNT][PRECEDENCE_RULE_SIZE];
#endif