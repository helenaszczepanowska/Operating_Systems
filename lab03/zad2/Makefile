CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY: all clean

all: search

search: search.c
	$(CC) $(CFLAGS) -o search search.c

test:
	./search

clean:
	rm -f search

