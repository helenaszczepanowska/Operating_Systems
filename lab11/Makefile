.PHONY: all clean

all: server client

server: 
	gcc server.c -o server -pthread

client:
	gcc client.c -o client -pthread

clean: 
	rm server client
