CC=gcc
CFLAGS=-Wall -g

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c

clean:
	rm -f main main_2

test: main
	./main 6 2

.PHONY: all clean 