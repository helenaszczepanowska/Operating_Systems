CC = gcc
CFLAGS = -Wall

all: life
	
life: grid.c life.c
	$(CC) $(CFLAGS) grid.c life.c -o life -lncurses -lpthread
	
test:
	./life 4
clean:
	rm -f life
