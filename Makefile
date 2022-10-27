CFLAGS=-std=c11 -Wall -Wextra -pedantic
CC=gcc

all: parser scanner

parser: parser.o scanner.o dynamic_string.o token.o error.o
	$(CC) $(CFLAGS) -o $@ $^

scanner: scanner.o dynamic_string.o token.o error.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f scanner parser *.o

run:
	make scanner
	cat ./tests/test1.php | ./scanner > ./tests/res1.txt

# TEST symtable
symtable_test: symtable_test.o symtable.o
	$(CC) $(CFLAGS) -g $^ -o $@

TEST_symtable: symtable_test
	valgrind --leak-check=full --show-leak-kinds=all ./symtable_test

TEST_symtable_clean:
	rm -f symtable_test *.o

# TEST token array
token_array_test: token_array_test.o token_array.o token.o
	$(CC) $(CFLAGS) -g $^ -o $@

TEST_token_array: token_array_test
	valgrind --leak-check=full --show-leak-kinds=all ./token_array_test

TEST_token_array_clean:
	rm -f token_array_test *.o

# TEST precedence parser
precedence_parser_test: precedence_parser_test.o precedence_parser.o precedence_stack.o precedence_rules.o precedence_table.o token_array.o token.o
	$(CC) $(CFLAGS) -g $^ -o $@

TEST_precedence_parser: precedence_parser_test
	valgrind --leak-check=full --show-leak-kinds=all ./precedence_parser_test

TEST_precedence_parser_clean:
	rm -f precedence_parser_test *.o

# TEST abstract_syntax_tree
abstract_syntax_tree_test: abstract_syntax_tree_test.o abstract_syntax_tree.o token_array.o token.o
	$(CC) $(CFLAGS) -g $^ -o $@

TEST_abstract_syntax_tree: abstract_syntax_tree_test
	valgrind --leak-check=full --show-leak-kinds=all ./$^

TEST_abstract_syntax_tree_clean:
	rm -f abstract_syntax_tree_test *.o

