INCLUDES=-I include/
CPP_DEBUG_FLAGS=-std=c++11 -g -lm $(INCLUDES)
CPPFLAGS=-std=c++11 -O2 -lm $(INCLUDES)

build:
	g++ main.cpp $(CPPFLAGS)

debug:
	g++ main.cpp $(CPP_DEBUG_FLAGS)

clean:
	rm a.out
