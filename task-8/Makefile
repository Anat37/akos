CC=gcc
CFLAGS= -Wall -g -ansi -pedantic

BINARIES= first second third

all: compile

compile: first.c second.c third.c
	$(CC) $(CFLAGS) -o first first.c 
	$(CC) $(CFLAGS) -o second second.c 
	$(CC) $(CFLAGS) -o third third.c 

clean:
	rm first
	rm second
	rm third

.PHONY: compile

