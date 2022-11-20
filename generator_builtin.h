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

#define BUILIN_FLOATVAL ""
#define BUILIN_INTVAL ""
#define BUILIN_STRVAL ""
#define BUILIN_STRLEN ""
#define BUILIN_SUBSTRING ""
#define BUILIN_ORD ""
#define BUILIN_CHR ""


#endif
