CPP=g++
CPPFLAGS=-Wall -g

all: compile

%.o : %.cpp %.h
	$(CPP) $(CPPFLAGS) -c $<

compile: text.cpp T_Args.o T_String.o Lists.o
	$(CPP) $(CPPFLAGS) text.cpp T_Args.o T_String.o Lists.o

clean:
	rm -rf a.out *.o *.dSYM