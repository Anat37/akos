CC = gcc

CFLAGS = -g -pedantic 
SOURCES = safe_gets.c signals.c bitwise_print.c

PROGRAMS = $(SOURCES:.c= ) 

OBJ = $(SOURCES:.c=.o)

all: signals bitwise_print safe_gets.o

signals: signals.o
	$(CC) signals.o -o signals
	
bitwise_print: bitwise_print.o
	$(CC) bitwise_print.o -o bitwise_print

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -rf $(OBJ) $(PROGRAMS)
