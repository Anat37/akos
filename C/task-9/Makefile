FLAGS= -ansi -Wall -pedantic -g

all: compile

compile: 
	gcc $(FLAGS) -o server server.c -lpthread
	gcc $(FLAGS) -o client client.c

clean: server client
	rm -rf server client client.dSYM server.dSYM	
