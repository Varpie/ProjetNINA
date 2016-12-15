CXX=g++
CXXFLAGS=-c -Wall
LDLIBS= -lpython2.7
SOURCES=main.cpp html_work/navigator.cpp html_work/intelligence.cpp timed_keystrokes/timed_keystrokes.c uinput/write_keyboard.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm *.o

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDLIBS) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@
