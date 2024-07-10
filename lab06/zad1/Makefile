CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY: all clean

all: main test

main: main.c
	$(CC) $(CFLAGS) -o main main.c

test:
	./main 0.000000001 2
	./main 0.000000001 4
	./main 0.0000000001 2
	./main 0.0000000001 4
	./main 0.0000000001 10


clean:
	rm -f main