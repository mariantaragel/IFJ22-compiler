#include <stdlib.h>  // size_t
#include <stdbool.h> // bool
#include "token.h"

/**
 * @brief Type of symbol.
 */
typedef enum {VAR_SYMBOL, FUNC_SYMBOL} symbol_type_t;

/**
 * @brief Symbol structure data type.
 * 
 */
typedef struct {
	symbol_type_t symbol_type;

	bool defined;

	bool variable_param_count;
	size_t param_count;
	int* param_types_array;	

	int return_type;
} symbol_info_t;

