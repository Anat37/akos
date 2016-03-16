CC=gcc

CFLAGS=-g -ansi -pedantic -w -Wall -Wextra -Werror -c

all: dna_packer.o
	$(CC) dna_packer.o -o packer

dna_packer.o: dna_packer.c
	$(CC) $(CFLAGS) dna_packer.c

clean:
	rm -rf *.o packer 
