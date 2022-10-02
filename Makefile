CFLAGS=-std=c11 -Wall -Wextra -Werror -pedantic
CC=gcc

scanner: scanner.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f scanner *.o
