#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "precedence_parser.h"
#include "precedence_stack.h"
#include "precedence_rules.h"

int main(){
    printf("%d", match_precedence_rule(PREC_RULE_ELEM_EXPR, PREC_RULE_ELEM_TERM(ADD), PREC_RULE_ELEM_EXPR));
}