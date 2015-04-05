C=g++
CFLAGS=-Wall -g

all: compile

T_Args.o: T_Args.cpp
	$(C) -c T_Args.cpp

compile: text.cpp T_Args.o
	$(C) $(CFLAGS) text.cpp T_Args.o 

clean:
	rm -rf a.out *.o *.dSYM