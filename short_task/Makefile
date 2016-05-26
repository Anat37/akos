CC = gcc

CFLAGS = -g -pedantic 
SOURCES = safe_gets.c signals.c bitwise_print.c wcsort.c msgclient.c mipt9a.c

PROGRAMS = $(SOURCES:.c= ) 

OBJ = $(SOURCES:.c=.o)

all: signals bitwise_print wcsort msgclient msgserver safe_gets.o

signals: signals.o
	$(CC) signals.o -o signals
	
bitwise_print: bitwise_print.o
	$(CC) bitwise_print.o -o bitwise_print

wcsort: wcsort.o
	$(CC) wcsort.o -o wcsort
	
msgclient: msgclient.o
	$(CC) msgclient.o -o msgclient
	
msgserver: mipt9a.o
	$(CC) mipt9a.o -o msgserver

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -rf $(OBJ) $(PROGRAMS)
