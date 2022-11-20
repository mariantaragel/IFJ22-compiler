CFLAGS= -std=c11 -Wall -Wextra -pedantic -g
LFLAGS= -lm
CC=gcc

all: main

main: main.o parser.o scanner.o dynamic_string.o token.o error.o abstract_syntax_tree.o token_array.o semantic_analyzer.o symtable.o precedence_parser.o precedence_stack.o precedence_rules.o precedence_table.o token_array.o token.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

parser: parser.o scanner.o dynamic_string.o token.o error.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

scanner: scanner.o dynamic_string.o token.o error.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

codegen: codegen.o dynamic_string.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

expgen: expgen.o dynamic_string.o scanner.o token.o error.o token_array.o generator.o semantic_analyzer.o symtable.o abstract_syntax_tree.o labelgen.o precedence_parser.o precedence_stack.o precedence_rules.o precedence_table.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f scanner parser parser_test codegen expgen main *.o

run:
	make scanner
	cat ./tests/test1.php | ./scanner > ./tests/res1.txt

parser_test: parser_test.o parser.o scanner.o dynamic_string.o token.o error.o abstract_syntax_tree.o token_array.o precedence_parser.o precedence_stack.o precedence_rules.o precedence_table.o
	$(CC) $(CFLAGS) -g $^ -o $@ $(LFLAGS)

# TEST symtable
symtable_test: symtable_test.o symtable.o
	$(CC) $(CFLAGS) $^ -o $@

TEST_symtable: symtable_test
	valgrind --leak-check=full --show-leak-kinds=all ./symtable_test

# TEST token array
token_array_test: token_array_test.o token_array.o token.o
	$(CC) $(CFLAGS) $^ -o $@

TEST_token_array: token_array_test
	valgrind --leak-check=full --show-leak-kinds=all ./token_array_test

# TEST precedence parser
precedence_parser_test: precedence_parser_test.o precedence_parser.o precedence_stack.o precedence_rules.o precedence_table.o scanner.o dynamic_string.o error.o token_array.o token.o
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

TEST_precedence_parser: precedence_parser_test
	valgrind --leak-check=full --show-leak-kinds=all ./precedence_parser_test

# TEST abstract_syntax_tree
abstract_syntax_tree_test: abstract_syntax_tree_test.o abstract_syntax_tree.o token_array.o token.o
	$(CC) $(CFLAGS) $^ -o $@

TEST_abstract_syntax_tree: abstract_syntax_tree_test
	valgrind --leak-check=full --show-leak-kinds=all ./$^

# TEST semantic analyzer
semantic_analyzer_test: semantic_analyzer_test.o semantic_analyzer.o symtable.o abstract_syntax_tree.o token_array.o token.o error.o
	$(CC) $(CFLAGS) $^ -o $@

TEST_semantic_analyzer: semantic_analyzer_test
	valgrind --leak-check=full --show-leak-kinds=all ./$^

# TEST generator
generator_test: generator_test.o generator.o generator_tools.o semantic_analyzer.o symtable.o abstract_syntax_tree.o generator_tools.o dynamic_string.o token_array.o token.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

TEST_generator: generator_test
	valgrind --leak-check=full --show-leak-kinds=all ./$^

# TEST clean
tclean:
	rm -f generator_test semantic_analyzer_test abstract_syntax_tree_test precedence_parser_test token_array_test *.o

