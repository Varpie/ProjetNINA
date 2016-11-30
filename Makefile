CXX=g++
CXXFLAGS=-c -Wall
LDLIBS= -lpython2.7
SOURCES=main.cpp html_work/navigator.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm *.o

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDLIBS) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
