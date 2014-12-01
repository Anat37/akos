CC=gcc
CFLAGS= -Wall -ansi -pedantic

all: shell

libdict.a: dictionary.o
	ar rc libdict.a dictionary.o
	ranlib libdict.a

libstrarr.a: strarr.o
	ar rc libstrarr.a strarr.o
	ranlib libstrarr.a

shell: libdict.a libstrarr.a
	$(CC) $(CFLAGS) -o shell shell.c libdict.a libstrarr.a

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o *.a ./shell

