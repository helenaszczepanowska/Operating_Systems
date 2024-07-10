CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY: all clean

all: integral

integral: integral_parameters.c
	$(CC) $(CFLAGS) -o integral_parameters integral_parameters.c
	$(CC) $(CFLAGS) -o integral_counter integral_counter.c


counter:
	./integral_counter

parameters:
	./integral_parameters

clean:
	rm -f main