CPP = g++
CPPFLAGS = -Wall -g
SOURCES = T_List.cpp Header.cpp Ordered_List.cpp Unordered_List.cpp Paragraph.cpp T_String.cpp T_Args.cpp
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES) )

all: libdata.a

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^

libdata.a: $(OBJECTS)
	ar -cr libdata.a $(OBJECTS)
	ranlib libdata.a

clean:
	rm  -f libdata.a $(OBJECTS)
	