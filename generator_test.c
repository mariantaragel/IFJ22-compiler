#include "generator.h"
#include "generator_tools.h"
#include "semantic_analyzer.h"
#include "abstract_syntax_tree.h"
#include "error.h"
#include "dynamic_string.h"

/*
typedef enum{
MOVE,CREATEFRAME,PUSHFRAME,POPFRAME,DEFVAR,CALL,RETURN,PUSHS,POPS,CLEARS,ADD,SUB,MUL,DIV,IDIV,ADDS,SUBS,MULS,
DIVS,IDIVS,LT,GT,EQ,LTS,GTS,EQS,AND,OR,NOT,ANDS,ORS,NOTS,INT2FLOAT,INT2CHAR,STRI2INT,INT2FLOATS,FLOAT2INTS,INT2CHARS,STRI2INTS,
READ,WRITE,CONCAT,STRLEN,GETCHAR,SETCHAR,TYPE,LABEL,JUMP,JUMPIFEQ,JUMPIFNEQ,JUMPIFEQS,JUMPIFNEQS,EXIT,BREAK,DPRINT
}instr_t;*/

int main(){

	dynamic_string_t * ds = ds_strinit("a normal string");
	ds_concat_str(ds, "MOVE,CREATEFRAME,PUSHFRAME,POPFRAME,DEFVAR,CALL,RETURN,PUSHS,POPS,CLEARS,ADD,SUB,MUL,DIV,IDIV,ADDS,SUBS,MULS, \
DIVS,IDIVS,LT,GT,EQ,LTS,GTS,EQNOT,ANDS,ORS,NOTS,INT2FL");
	ds_concat_str(ds, "_________");
	ds_write_uint(ds, 19900);
	ds_concat_str(ds, "_________");

ds_concat_str(ds, "OAT,INT2CHAR,STRI2INT,INT2FLOATS,FLOAT2INTS,INT2,PIFNEQ,JUMP T,ANDS,ORS,NOTS,INT2FLOAT,INT2CHAR,STRI2INT,INT2FLOATS,FLOAT2INTS,INT2CHARS,STRI2INTS, \
READ,WRITE,CONCAT,STRLEN,GETCHAR,SETCHAR,TYPE,LABEL,JUMP,JUMPIFEQ,JUMPIFNEQ,JUMPIF");
	ds_write_uint(ds, 2);
	printf("%s\n", ds->str);
	printf("size %ld\nlength %ld\n", ds->size, strlen(ds->str));
	ds_dstr(ds);

	/*
	AST_node_t* prog_n = AST_create_node(PROG_N);
	if(prog_n == NULL) return 2;

	semantic_analyzer(prog_n);

	if(error != OK) return error;

	generator(prog_n);

	if(error != OK) return error;

	AST_free(prog_n);*/
}