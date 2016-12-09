CXX=g++
CC=gcc
CXXFLAGS=-c -Wall
CFLAGS=-c -Wall -lm
LDLIBS= -lpython2.7
SOURCES=main.cpp html_work/navigator.cpp html_work/intelligence.cpp timed_keystrokes/timed_keystrokes.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm *.o

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDLIBS) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
