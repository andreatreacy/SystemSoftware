CC = gcc
CFLAGS = -pthread

server: server.c
	$(CC) -o server server.c $(CFLAGS)

client: client.c
	$(CC) -o client client.c

run-server: server
	./server

run-client: client
	./client

clean:
	rm -f server client