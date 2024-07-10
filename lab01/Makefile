CXX = gcc
CFLAGS = -Wall -g -std=c17

.PHONY: all countdown clean

all: countdown

countdown: countdown.c
	$(CXX) $(CFLAGS) -o countdown countdown.c

test:
	./countdown

clean:
	rm -f countdown

