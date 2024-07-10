CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY:  all clean ignore handler mask none

all: signal

signal: signal.c
	$(CC) $(CFLAGS) -o signal signal.c

ignore:
	./signal ignore

handler:
	./signal handler

mask:
	./signal mask

none:
	./signal none


clean:
	rm -f signal

