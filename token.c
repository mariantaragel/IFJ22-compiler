/****
 ** token.c
 ** Řešení IFJ-PROJ, 7.10.2022
 ** Autor: xhorva17
 ** Přeloženo:
 **/

/**
 * @file token.c
 * @author xhorva17
 * @brief Token implementation.
 * @date 2022-10-07
 */

#include <stdlib.h>
#include "token.h"


token_t * t_init() {
	return NULL;
}


void t_dstr(token_t * t) {
	free(t);
	return;
}
