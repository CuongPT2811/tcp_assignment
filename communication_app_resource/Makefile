CC = gcc
CFLAGS = -Wall -Wextra -O2

CLIENT = TCPClient
SERVER = TCPServer

all: $(CLIENT) $(SERVER)

$(CLIENT): TCPClient.c
	$(CC) $(CFLAGS) -o $(CLIENT) TCPClient.c -pthread -lm

$(SERVER): TCPServer.c
	$(CC) $(CFLAGS) -o $(SERVER) TCPServer.c -pthread -lm

clean:
	rm -f $(CLIENT) $(SERVER)