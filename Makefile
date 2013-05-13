CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=-lGLEW -lGL -lglut -lGLU -fpermissive -lpng

HEADERS= glext.h glInfo.h framebuffer.h textfile.h glslProgram.h texture.hpp 
SOURCES= glInfo.cpp framebuffer.cpp textfile.cpp glslProgram.cpp main.cpp texture.cpp 

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
