CC=gcc
CFLAGS= -Wall -ansi -pedantic

all: shell

libdata.a: *.o
	ar -cr libdata.a *.o
	ranlib libdata.a

shell: libdata.a
	$(CC) $(CFLAGS) -o shell shell.c libdata.a

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o *.a ./shell

