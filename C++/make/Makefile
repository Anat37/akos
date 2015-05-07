GPPFLAGS=-Wall -ansi -pedantic

all: compile

Types.o: Types.cpp
	g++ $(GPPFLAGS) -c Types.cpp

compile: make.cpp Types.o
	g++ $(GPPFLAGS) make.cpp Types.o

clean:
	rm a.out *.o