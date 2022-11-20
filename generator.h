#include <stdbool.h>
#include "abstract_syntax_tree.h"
#include "error.h"
#include "labelgen.h"

#ifndef GENERATOR_H
#define GENERATOR_H


void generator(AST_node_t* root);

typedef struct{
    bool is_in_function;
	char* func_end_label;
}generator_context_t;

extern size_t ind;


void G(const char* fmt, ...);
void dec_ind();
void inc_ind();
error_codes_t gen_var_def_check(char* var_name, generator_context_t* gen_context);
error_codes_t gen_expr_vars_def_checks(AST_node_t* expr_n, generator_context_t* gen_context);
error_codes_t gen_expr(AST_node_t* expr_n, generator_context_t* gen_context);
error_codes_t gen_true_false_jump(char* true_label, char* false_label);
error_codes_t gen_while(AST_node_t* while_n, generator_context_t* gen_context);
error_codes_t gen_if(AST_node_t* if_n, generator_context_t* gen_context);
error_codes_t gen_ass_expr(AST_node_t* ass_expr_n, generator_context_t* gen_context);
error_codes_t gen_func_args_def_checks(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_func_args_push(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_func_call(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_ass_func(AST_node_t* ass_func_n, generator_context_t* gen_context);
error_codes_t gen_standalone_func_call(AST_node_t* func_call_n, generator_context_t* gen_context);
error_codes_t gen_standalone_expr(AST_node_t* expr_n, generator_context_t* gen_context);
error_codes_t gen_body(AST_node_t* body_n, generator_context_t* gen_context);
error_codes_t gen_func_params(AST_node_t* params_n, char* func_name);
error_codes_t gen_func_def(AST_node_t* func_def_n, generator_context_t* gen_context);
error_codes_t gen_missing_return();
error_codes_t gen_return(AST_node_t* return_n, generator_context_t* gen_context);
error_codes_t gen_func_def_flags(AST_node_t* used_func_list_n);
error_codes_t gen_built_in_functions(AST_node_t* used_func_list_n);
error_codes_t gen_var_defs(AST_node_t* used_vars_list_n, generator_context_t* gen_context);
error_codes_t gen_prog(AST_node_t* prog_n, generator_context_t* gen_context);
generator_context_t* generator_context_create();
void generator_context_free(generator_context_t* gen_context);

#endif