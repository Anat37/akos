CC = gcc
CFLAGS = -Wall -ansi -pedantic
SUBDIR = struct

all: libdata.a

libdata.a: *.o
	ar -cr libdata.a *.o
	ranlib libdata.a

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o *.a ./shell
