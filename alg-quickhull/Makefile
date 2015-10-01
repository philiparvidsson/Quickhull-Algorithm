CC = gcc
CFLAGS = -c -Wall -Os -Isrc

LDFLAGS=
LDLIBS=-lm -lGL -lX11

SOURCES=$(shell find src -type f -iname '*.c')
OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)

all: $(OBJECTS)
	mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o bin/alg-quickhull

clean:
	rm -rf bin
	rm -f $(TARGET) $(OBJECTS)
