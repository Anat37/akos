CC=gcc
CFLAGS= -ansi -pedantic

all: compile

compile: first.c second.c
	$(CC) $(CFLAGS) -o first first.c 
	$(CC) $(CFLAGS) -o second second.c 
	$(CC) $(CFLAGS) -o third third.c 

clean:
	rm first
	rm second
	rm third
