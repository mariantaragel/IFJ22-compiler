#include "semantic_analyzer.h"
#include "abstract_syntax_tree.h"

int main(){
	AST_node_t* prog_n = AST_create_node(PROG_N);
	if(prog_n == NULL) return 2;

	semantic_analyzer(prog_n);

	AST_free(prog_n);
}