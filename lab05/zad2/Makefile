CC = gcc
CFLAGS = -g -Wall -std=c17

.PHONY:  all clean 

all: sender catcher

sender: sender.c
	$(CC) $(CFLAGS) -o sender sender.c

catcher: catcher.c
	$(CC) $(CFLAGS) -o catcher catcher.c

run_catcher:
	./catcher 

clean:
	rm -f sender catcher

