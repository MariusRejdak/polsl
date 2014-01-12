CC=g++
CFLAGS=-std=c++11 -Wall -W -O3
LIBS_M=Graph.cpp BoostGraphWrapper.cpp main.cpp
LIBS_T=Graph.cpp BoostGraphWrapper.cpp test.cpp
OBJECTS_M=$(LIBS_M:.cpp=.o)
OBJECTS_T=$(LIBS_T:.cpp=.o)

all: $(LIBS_M) main.out test.out

test: $(LIBS_T) test.out

main.out: $(OBJECTS_M) 
	$(CC) $(CFLAGS) $(OBJECTS_M) -o $@

test.out: $(OBJECTS_T) 
	$(CC) $(CFLAGS) $(OBJECTS_T) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.out
