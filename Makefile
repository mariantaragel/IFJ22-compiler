CFLAGS=-std=c11 -Wall -Wextra -pedantic
CC=gcc

scanner: scanner.o dynamic_string.o token.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f scanner *.o

run:
	make scanner
	cat ./tests/test1.php | ./scanner > ./tests/res1.txt


# TEST symtable
symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -g -c symtable.c -o $@

symtable_test.o: symtable_test.c symtable.h
	$(CC) $(CFLAGS) -g -c symtable_test.c -o $@

symtable_test: symtable_test.o symtable.o
	$(CC) $(CFLAGS) -g symtable_test.o symtable.o -o $@

symtable_test_run: symtable_test
	valgrind --leak-check=full --show-leak-kinds=all ./symtable_test

symtable_test_clean:
	rm -f symtable_test *.o
