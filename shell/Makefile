CC=gcc
CFLAGS= -Wall -ansi -pedantic -g
LIBDIR= -L.

all: shell

libdict.a: dictionary.o
	ar rc libdict.a dictionary.o
	ranlib libdict.a

libstrarr.a: strarr.o
	ar rc libstrarr.a strarr.o
	ranlib libstrarr.a


shell: libdict.a libstrarr.a shell.o
	$(CC) $(CFLAGS) -o shell $(LIBDIR) -ldict -lstrarr shell.o

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o *.a ./shell

