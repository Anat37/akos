

all: server client

client: client.o
	gcc -g client.o
	
server: server.o
	gcc -g server.o -o server
	
client.o: client.c
	gcc -g -c client.c -o client.o
	
server.o: server.c
	gcc -g -c server.c -o server.o
	
clean:
	server.o client.o server client
