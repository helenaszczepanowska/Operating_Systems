CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY: all test clean

all: create_process

create_process: create_process.c
	$(CC) $(CFLAGS) -o create_process create_process.c

test:
	./create_process 5

clean:
	rm -f create_process

