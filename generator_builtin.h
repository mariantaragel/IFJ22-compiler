/**
 * @file generator_builtin.h
 * @author David Klajbl (xklajb00@stud.fit.vutbr.cz)
 * @brief Definitions of macros containing source code of built-in functions and helper functions of generated IFJcode22 program.
 * @date 2022-11-26
 * 
 */

#ifndef GENERATOR_BUILTIN_H
#define GENERATOR_BUILTIN_H

// function readi() : ?int
// reads integer from stdin
#define BUILTIN_READI "\
LABEL readi												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	# check if number of arguments is 0					\n\
		POPS GF@_arg_count								\n\
		JUMPIFEQ ?readi?arg_count_ok GF@_arg_count int@0\n\
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

// function readf() : ?float
// reads float from stdin
#define BUILTIN_READF "\
LABEL readf												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	# check if number of arguments is 0					\n\
		POPS GF@_arg_count								\n\
		JUMPIFEQ ?readf?arg_count_ok GF@_arg_count int@0\n\
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

// function reads() : ?string
// reads string from stdin
#define BUILTIN_READS "\
LABEL reads												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	# check if number of arguments is 0					\n\
		POPS GF@_arg_count								\n\
		JUMPIFEQ ?reads?arg_count_ok GF@_arg_count int@0\n\
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

// function write ( term_1 , term_2 , ..., term_n ) : void
// prints values of term_1 to term_n to stdout
#define BUILTIN_WRITE "\
LABEL write												\n\
CREATEFRAME												\n\
PUSHFRAME												\n\
	DEFVAR LF@cur_arg									\n\
														\n\
	# get number of arguments							\n\
	POPS GF@_arg_count									\n\
														\n\
	# iterate over all arguments and print them			\n\
	LABEL ?write?while_start							\n\
	JUMPIFEQ ?write?while_end GF@_arg_count int@0		\n\
														\n\
		SUB GF@_arg_count GF@_arg_count int@1			\n\
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

// function floatval(term) : float
// returns value of term converted to float
#define BUILTIN_FLOATVAL "\
LABEL floatval											\n\
	# check if number of arguments is 1					\n\
	POPS GF@_arg_count									\n\
	JUMPIFEQ ?floatval?arg_count_ok GF@_arg_count int@1	\n\
		EXIT int@4										\n\
	LABEL ?floatval?arg_count_ok						\n\
														\n\
	# get argument										\n\
	POPS GF@_tmp										\n\
														\n\
	TYPE GF@_tmp_type GF@_tmp							\n\
														\n\
	JUMPIFNEQ ?floatval?next1 GF@_tmp_type string@nil	\n\
		MOVE GF@_tmp_res float@0x0p+0					\n\
		JUMP ?floatval?end								\n\
														\n\
	LABEL ?floatval?next1								\n\
	JUMPIFNEQ ?floatval?next2 GF@_tmp_type string@int	\n\
		INT2FLOAT GF@_tmp_res GF@_tmp					\n\
		JUMP ?floatval?end								\n\
														\n\
	LABEL ?floatval?next2								\n\
	JUMPIFNEQ ?floatval?next3 GF@_tmp_type string@float	\n\
		MOVE GF@_tmp_res GF@_tmp						\n\
		JUMP ?floatval?end								\n\
														\n\
	LABEL ?floatval?next3								\n\
	JUMPIFNEQ ?floatval?next4 GF@_tmp_type string@string\n\
		EXIT int@8										\n\
														\n\
	LABEL ?floatval?next4								\n\
		JUMPIFEQ ?floatval?false GF@_tmp bool@false		\n\
			MOVE GF@_tmp_res float@0x1p+0				\n\
			JUMP ?floatval?end							\n\
		LABEL ?floatval?false							\n\
			MOVE GF@_tmp_res float@0x0p+0				\n\
														\n\
	LABEL ?floatval?end									\n\
	PUSHS GF@_tmp_res									\n\
RETURN													\n\
"



// function intval(term) : int
// returns value of term converted to int
#define BUILTIN_INTVAL "\
LABEL intval											\n\
	# check if number of arguments is 1					\n\
	POPS GF@_arg_count									\n\
	JUMPIFEQ ?intval?arg_count_ok GF@_arg_count int@1	\n\
		EXIT int@4										\n\
	LABEL ?intval?arg_count_ok							\n\
														\n\
	# get argument										\n\
	POPS GF@_tmp										\n\
														\n\
	TYPE GF@_tmp_type GF@_tmp							\n\
														\n\
	JUMPIFNEQ ?intval?next1 GF@_tmp_type string@nil		\n\
		MOVE GF@_tmp_res int@0							\n\
		JUMP ?intval?end								\n\
														\n\
	LABEL ?intval?next1									\n\
	JUMPIFNEQ ?intval?next2 GF@_tmp_type string@int		\n\
		MOVE GF@_tmp_res GF@_tmp						\n\
		JUMP ?intval?end								\n\
														\n\
	LABEL ?intval?next2									\n\
	JUMPIFNEQ ?intval?next3 GF@_tmp_type string@float	\n\
		FLOAT2INT GF@_tmp_res GF@_tmp					\n\
		JUMP ?intval?end								\n\
														\n\
	LABEL ?intval?next3									\n\
	JUMPIFNEQ ?intval?next4 GF@_tmp_type string@string	\n\
		EXIT int@8										\n\
														\n\
	LABEL ?intval?next4									\n\
		JUMPIFEQ ?intval?false GF@_tmp bool@false		\n\
			MOVE GF@_tmp_res int@1						\n\
			JUMP ?intval?end							\n\
		LABEL ?intval?false								\n\
			MOVE GF@_tmp_res int@0						\n\
														\n\
	LABEL ?intval?end									\n\
	PUSHS GF@_tmp_res									\n\
RETURN													\n\
"

// function strval(term) : string
// returns value of term converted to string
#define BUILTIN_STRVAL "\
LABEL strval											\n\
	# check if number of arguments is 1					\n\
	POPS GF@_arg_count									\n\
	JUMPIFEQ ?strval?arg_count_ok GF@_arg_count int@1	\n\
		EXIT int@4										\n\
	LABEL ?strval?arg_count_ok							\n\
														\n\
	# get argument										\n\
	POPS GF@_tmp										\n\
														\n\
	TYPE GF@_tmp_type GF@_tmp							\n\
														\n\
	JUMPIFNEQ ?strval?next1 GF@_tmp_type string@nil		\n\
		MOVE GF@_tmp_res string@						\n\
		JUMP ?strval?end								\n\
														\n\
	LABEL ?strval?next1									\n\
	JUMPIFNEQ ?strval?next2 GF@_tmp_type string@int		\n\
		EXIT int@8										\n\
														\n\
	LABEL ?strval?next2									\n\
	JUMPIFNEQ ?strval?next3 GF@_tmp_type string@float	\n\
		EXIT int@8										\n\
														\n\
	LABEL ?strval?next3									\n\
	JUMPIFNEQ ?strval?next4 GF@_tmp_type string@string	\n\
		MOVE GF@_tmp_res GF@_tmp						\n\
		JUMP ?strval?end								\n\
														\n\
	LABEL ?strval?next4									\n\
		JUMPIFEQ ?strval?false GF@_tmp bool@false		\n\
			MOVE GF@_tmp_res string@1					\n\
			JUMP ?strval?end							\n\
		LABEL ?strval?false								\n\
			MOVE GF@_tmp_res string@					\n\
														\n\
	LABEL ?strval?end									\n\
	PUSHS GF@_tmp_res									\n\
RETURN													\n\
"

// function substring(string $s, int $i, int $j) : ?string
// returns substring of string $s, starting at index $i and ending at index $j-1
// function returns null if:
//   $i < 0 or $j < 0 or $i > $j or $i >= strlen($s) or $j > strlen($s)
#define BUILTIN_SUBSTRING "\
LABEL substring														\n\
CREATEFRAME															\n\
PUSHFRAME															\n\
	# check if number of arguments is 3								\n\
	POPS GF@_arg_count												\n\
	JUMPIFEQ ?substring?arg_count_ok GF@_arg_count int@3			\n\
		EXIT int@4													\n\
	LABEL ?substring?arg_count_ok									\n\
																	\n\
	DEFVAR LF@str													\n\
	POPS LF@str														\n\
	TYPE GF@_tmp_type LF@str										\n\
	JUMPIFEQ ?substring?str?param_type_ok GF@_tmp_type string@string\n\
		EXIT int@4													\n\
	LABEL ?substring?str?param_type_ok								\n\
																	\n\
	DEFVAR LF@i														\n\
	POPS LF@i														\n\
	TYPE GF@_tmp_type LF@i											\n\
	JUMPIFEQ ?substring?i?param_type_ok GF@_tmp_type string@int		\n\
		EXIT int@4													\n\
	LABEL ?substring?i?param_type_ok								\n\
																	\n\
	DEFVAR LF@j														\n\
	POPS LF@j														\n\
	TYPE GF@_tmp_type LF@j											\n\
	JUMPIFEQ ?substring?j?param_type_ok GF@_tmp_type string@int		\n\
		EXIT int@4													\n\
	LABEL ?substring?j?param_type_ok								\n\
																	\n\
	LT GF@_tmp LF@i int@0 # if i < 0 return null					\n\
	JUMPIFEQ ?substring?return_null GF@_tmp bool@true				\n\
																	\n\
	LT GF@_tmp LF@j int@0 # if j < 0								\n\
	JUMPIFEQ ?substring?return_null GF@_tmp bool@true				\n\
																	\n\
	GT GF@_tmp LF@i LF@j # if i > j return null						\n\
	JUMPIFEQ ?substring?return_null GF@_tmp bool@true				\n\
																	\n\
	STRLEN GF@_tmp_res LF@str										\n\
	GT GF@_tmp LF@j GF@_tmp_res # if j > strlen(s) return null		\n\
	JUMPIFEQ ?substring?return_null GF@_tmp bool@true				\n\
	GT GF@_tmp LF@i GF@_tmp_res # if i > strlen(s) return null		\n\
	JUMPIFEQ ?substring?return_null GF@_tmp bool@true				\n\
	EQ GF@_tmp LF@i GF@_tmp_res # if i = strlen(s) return null		\n\
	JUMPIFEQ ?substring?return_null GF@_tmp bool@true				\n\
																	\n\
	MOVE GF@_tmp_res string@										\n\
																	\n\
	LABEL ?substring?while_start									\n\
	JUMPIFEQ ?substring?while_end LF@i LF@j							\n\
																	\n\
		GETCHAR GF@_tmp LF@str LF@i									\n\
		CONCAT GF@_tmp_res GF@_tmp_res GF@_tmp						\n\
		ADD LF@i LF@i int@1											\n\
																	\n\
	JUMP ?substring?while_start										\n\
	LABEL ?substring?while_end										\n\
																	\n\
	PUSHS GF@_tmp_res												\n\
	POPFRAME														\n\
	RETURN															\n\
																	\n\
	LABEL ?substring?return_null									\n\
	PUSHS nil@nil													\n\
	POPFRAME														\n\
	RETURN															\n\
"

// function strlen(string $s) : int
// returns length (number of characters) of string $s
#define BUILTIN_STRLEN "\
LABEL strlen													\n\
	# check if number of arguments is 1							\n\
	POPS GF@_arg_count											\n\
	JUMPIFEQ ?strlen?arg_count_ok GF@_arg_count int@1			\n\
		EXIT int@4												\n\
	LABEL ?strlen?arg_count_ok									\n\
																\n\
	# get argument												\n\
	POPS GF@_tmp												\n\
																\n\
	# check if parameter type is string							\n\
	TYPE GF@_tmp_type GF@_tmp									\n\
	JUMPIFEQ ?strlen?param_type_ok GF@_tmp_type string@string	\n\
		EXIT int@4												\n\
	LABEL ?strlen?param_type_ok									\n\
																\n\
	# get and return length of string							\n\
	STRLEN GF@_tmp GF@_tmp										\n\
	PUSHS GF@_tmp												\n\
RETURN															\n\
"

// function ord(string $c) : int
// returns ordinal value (ASCII) of first character in string $character
// if string $c is empty, 0 is returned
#define BUILTIN_ORD "\
LABEL ord														\n\
	# check if number of arguments is 1							\n\
	POPS GF@_arg_count											\n\
	JUMPIFEQ ?ord?arg_count_ok GF@_arg_count int@1				\n\
		EXIT int@4												\n\
	LABEL ?ord?arg_count_ok										\n\
																\n\
	# get argument												\n\
	POPS GF@_tmp												\n\
																\n\
	# check if parameter type is string							\n\
	TYPE GF@_tmp_type GF@_tmp									\n\
	JUMPIFEQ ?ord?param_type_ok GF@_tmp_type string@string		\n\
		EXIT int@4												\n\
	LABEL ?ord?param_type_ok									\n\
																\n\
	# check if string is not empty								\n\
	JUMPIFEQ ?ord?empty_string GF@_tmp string@					\n\
																\n\
	# string is not empty, return ASCII value of first character\n\
	STRI2INT GF@_tmp GF@_tmp int@0								\n\
	PUSHS GF@_tmp												\n\
	RETURN														\n\
																\n\
	# string is empty, return zero								\n\
	LABEL ?ord?empty_string										\n\
	PUSHS int@0													\n\
	RETURN														\n\
"

// function chr(int $i) : string
// returns string containing one character with ASCII code specified by parameter $i
#define BUILTIN_CHR "\
LABEL chr														\n\
	# check if number of arguments is 1							\n\
	POPS GF@_arg_count											\n\
	JUMPIFEQ ?chr?arg_count_ok GF@_arg_count int@1				\n\
		EXIT int@4												\n\
	LABEL ?chr?arg_count_ok										\n\
																\n\
	# get argument												\n\
	POPS GF@_tmp												\n\
																\n\
	# check if parameter type is int							\n\
	TYPE GF@_tmp_type GF@_tmp									\n\
	JUMPIFEQ ?chr?param_type_ok GF@_tmp_type string@int			\n\
		EXIT int@4												\n\
	LABEL ?chr?param_type_ok									\n\
																\n\
	# get and return length of string							\n\
	INT2CHAR GF@_tmp GF@_tmp									\n\
	PUSHS GF@_tmp												\n\
RETURN															\n\
"

// helper function to interpret expression result as true or false
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
		JUMPIFEQ ?&to_bool?false GF@_tmp string@0		\n\
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
