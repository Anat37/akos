all: compile

compile: 
	gcc -ansi -Wall -pedantic -o server server.c
	gcc -ansi -Wall -pedantic -o client client_server.c

clean: server client
	rm server client
