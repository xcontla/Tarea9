CC=g++
CFLAGS=-c -g -Wall
LDFLAGS=-lGLEW -lGL -lglut -lGLU -fpermissive -lpng -lm

HEADERS= glext.h glInfo.h framebuffer.h texture.hpp 
SOURCES= glInfo.cpp framebuffer.cpp main.cpp  texture.cpp 

OBJECTS=$(SOURCES:.cpp=.o)
OBJECTSGENERA=$(SOURCESGENERA:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE) *~
