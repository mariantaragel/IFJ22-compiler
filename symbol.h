#include <stdlib.h>	 // size_t
#include <stdbool.h> // bool
#include "token.h"

/**
 * @brief Type of symbol.
 */
typedef enum{
	VAR_SYMBOL,
	FUNC_SYMBOL
} symbol_type_t;

#ifndef DATATYPE_T
#define DATATYPE_T
/**
 * @brief Datatype type.
 * 	Its values are synonymous to FLT_T,STR_T,INT_T,VOID_T,NFLT_T,NINT_T,NSTR_T values of token_type_t.
 */
typedef int datatype_t;
#endif

/**
 * @brief Symbol structure data type.
 *
 */
typedef struct
{
	symbol_type_t symbol_type;

	bool defined;
		
	bool variable_param_count;
	
	size_t param_count;
	datatype_t *param_types_array;

	datatype_t return_type;
} symbol_info_t;
