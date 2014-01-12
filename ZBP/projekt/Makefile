CC=g++
CFLAGS=-std=c++11 -Wall -W -g
SOURCES=main.cpp Graph.cpp BoostGraphWrapper.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
    $(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -rf *.o *.out

run:
    ./main.out