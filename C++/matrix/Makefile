C=g++
CFLAGS=-Wall -g

all: compile

compile: complex.cpp
	$(C) $(CFLAGS) -c complex.cpp complex_matrix.cpp
	$(C) $(CFLAGS) -o matrices main.cpp complex_matrix.o complex.o

clean:
	rm -rf ./matrices *.o *.dSYM
