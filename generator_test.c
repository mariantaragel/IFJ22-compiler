#include "generator.h"
#include "generator_tools.h"
#include "semantic_analyzer.h"
#include "abstract_syntax_tree.h"
#include "error.h"

/*
typedef enum{
MOVE,CREATEFRAME,PUSHFRAME,POPFRAME,DEFVAR,CALL,RETURN,PUSHS,POPS,CLEARS,ADD,SUB,MUL,DIV,IDIV,ADDS,SUBS,MULS,
DIVS,IDIVS,LT,GT,EQ,LTS,GTS,EQS,AND,OR,NOT,ANDS,ORS,NOTS,INT2FLOAT,INT2CHAR,STRI2INT,INT2FLOATS,FLOAT2INTS,INT2CHARS,STRI2INTS,
READ,WRITE,CONCAT,STRLEN,GETCHAR,SETCHAR,TYPE,LABEL,JUMP,JUMPIFEQ,JUMPIFNEQ,JUMPIFEQS,JUMPIFNEQS,EXIT,BREAK,DPRINT
}instr_t;*/

int main(){
	char* label = gen_label("A",NULL,NULL, true);
	printf("%s", label);
	free(label);
	/*
	AST_node_t* prog_n = AST_create_node(PROG_N);
	if(prog_n == NULL) return 2;

	semantic_analyzer(prog_n);

	if(error != OK) return error;

	generator(prog_n);

	if(error != OK) return error;

	AST_free(prog_n);*/
}