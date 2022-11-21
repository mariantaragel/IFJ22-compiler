#ifndef GENERATOR_BUILTIN_H
#define GENERATOR_BUILTIN_H

#define BUILTIN_READI "\
LABEL readi												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	DEFVAR LF@arg_count									\n\
	# check if number of arguments is 0					\n\
		POPS LF@arg_count								\n\
		JUMPIFEQ ?readi?arg_count_ok LF@arg_count int@0	\n\
			EXIT int@4									\n\
		LABEL ?readi?arg_count_ok						\n\
														\n\
	# read int from input								\n\
		DEFVAR LF@read_int								\n\
		READ LF@read_int int							\n\
		PUSHS LF@read_int								\n\
POPFRAME												\n\
RETURN													\n\
"

#define BUILTIN_READF "\
LABEL readf												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	DEFVAR LF@arg_count									\n\
	# check if number of arguments is 0					\n\
		POPS LF@arg_count								\n\
		JUMPIFEQ ?readf?arg_count_ok LF@arg_count int@0	\n\
			EXIT int@4									\n\
		LABEL ?readf?arg_count_ok						\n\
														\n\
	# read float from input								\n\
		DEFVAR LF@read_float							\n\
		READ LF@read_float float						\n\
		PUSHS LF@read_float								\n\
POPFRAME												\n\
RETURN													\n\
"

#define BUILTIN_READS "\
LABEL reads												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	DEFVAR LF@arg_count									\n\
	# check if number of arguments is 0					\n\
		POPS LF@arg_count								\n\
		JUMPIFEQ ?reads?arg_count_ok LF@arg_count int@0	\n\
			EXIT int@4									\n\
		LABEL ?reads?arg_count_ok						\n\
														\n\
	# read string from input							\n\
		DEFVAR LF@read_string							\n\
		READ LF@read_string string						\n\
		PUSHS LF@read_string							\n\
POPFRAME												\n\
RETURN													\n\
"

#define BUILTIN_WRITE "\
LABEL write												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	DEFVAR LF@arg_count									\n\
	DEFVAR LF@cur_arg									\n\
														\n\
	# get number of arguments							\n\
	POPS LF@arg_count									\n\
														\n\
	# iterate over all arguments and print them			\n\
	LABEL ?write?while_start							\n\
	JUMPIFEQ ?write?while_end LF@arg_count int@0		\n\
														\n\
		SUB LF@arg_count LF@arg_count int@1				\n\
		POPS LF@cur_arg									\n\
		WRITE LF@cur_arg								\n\
														\n\
	JUMP ?write?while_start								\n\
	LABEL ?write?while_end								\n\
														\n\
	# return implicit null								\n\
	PUSHS nil@nil										\n\
POPFRAME												\n\
RETURN													\n\
"

#define BUILTIN_FLOATVAL ""
#define BUILTIN_INTVAL ""
#define BUILTIN_STRVAL ""
#define BUILTIN_STRLEN ""
#define BUILTIN_SUBSTRING ""
#define BUILTIN_ORD ""
#define BUILTIN_CHR ""

#define HELPER_TO_BOOL_ "\
LABEL &to_bool											\n\
	POPS GF@_tmp										\n\
	TYPE GF@_tmp_type GF@_tmp							\n\
														\n\
	JUMPIFNEQ ?&to_bool?next1 GF@_tmp_type string@nil	\n\
		JUMP ?&to_bool?false							\n\
														\n\
	LABEL ?&to_bool?next1								\n\
	JUMPIFNEQ ?&to_bool?next2 GF@_tmp_type string@int	\n\
		JUMPIFEQ ?&to_bool?false GF@_tmp int@0			\n\
		JUMP ?&to_bool?true								\n\
														\n\
	LABEL ?&to_bool?next2								\n\
	JUMPIFNEQ ?&to_bool?next3 GF@_tmp_type string@float	\n\
		JUMPIFEQ ?&to_bool?false GF@_tmp float@0x0p+0	\n\
		JUMP ?&to_bool?true								\n\
														\n\
	LABEL ?&to_bool?next3								\n\
	JUMPIFNEQ ?&to_bool?next4 GF@_tmp_type string@string\n\
		JUMPIFEQ ?&to_bool?false GF@_tmp string@		\n\
		JUMP ?&to_bool?true								\n\
														\n\
	LABEL ?&to_bool?next4								\n\
		JUMPIFEQ ?&to_bool?false GF@_tmp bool@false		\n\
														\n\
    LABEL ?&to_bool?true								\n\
        PUSHS bool@true									\n\
        RETURN											\n\
														\n\
    LABEL ?&to_bool?false								\n\
        PUSHS bool@false								\n\
        RETURN											\n\
"


#endif
