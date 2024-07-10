CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY: all clean

all: reverse reverse_1024

reverse: reverse.c
	$(CC) $(CFLAGS) -o reverse reverse.c -D BYTES

reverse_1024: reverse.c
	$(CC) $(CFLAGS) -o reverse_1024 reverse.c

test_one:
	./reverse file.txt result.txt

test_1024:
	./reverse_1024 file.txt result.txt

clean:
	rm -f reverse reverse_1024 result.txt pomiar_zad_2.txt

