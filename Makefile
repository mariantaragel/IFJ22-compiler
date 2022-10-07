CFLAGS=-std=c11 -Wall -Wextra
CC=gcc

scanner: scanner.o dynamic_string.o token.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f scanner *.o

run:
	make scanner
	cat ./tests/test1.php | ./scanner > ./tests/res1.txt
