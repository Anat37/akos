CPP=g++
CPPFLAGS=-Wall -g
SUBDIR = lib

all: libs compile

libs:
	make -C lib libdata.a

compile: text.cpp
	$(CPP) $(CPPFLAGS) text.cpp $(SUBDIR)/libdata.a

clean:
	make -C lib clean
	rm -rf a.out *.dSYM