CC = gcc

CFLAGS = -g -ansi -pedantic -w

SOURCES = safe_gets.c 

PROGRAMS = $(SOURCES:.c= ) 

OBJ = $(SOURCES:.c=.o)

all: $(PROGRAMS)


$(PROGRAMS): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -rf $(OBJ) $(PROGRAMS)
