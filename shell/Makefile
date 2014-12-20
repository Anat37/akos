CC = gcc
CFLAGS = -Wall -ansi -pedantic
SUBDIR = struct

all: shell

shell:
	cd $(SUBDIR);make
	$(CC) $(CFLAGS) -o shell shell.c $(SUBDIR)/libdata.a

clean:
	rm ./shell
	cd $(SUBDIR);make clean
