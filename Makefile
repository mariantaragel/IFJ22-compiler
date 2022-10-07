CFLAGS=-std=c11 -Wall -Wextra -Werror -pedantic
CC=gcc

scanner: scanner.o dynamic_string.o token.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f scanner *.o

run:
	make scanner
	cat test.php | ./scanner
