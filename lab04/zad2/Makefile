CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY: all clean

all: processes

processes: processes.c
	$(CC) $(CFLAGS) -o processes processes.c

test:
	./processes test_directory

clean:
	rm -f processes

