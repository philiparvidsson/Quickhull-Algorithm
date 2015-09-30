CC = gcc
CFLAGS = -c -Wall -Os -Isrc

LDFLAGS=
LDLIBS=-lm -lGL -lX11

SOURCES=$(shell find src -type f -iname '*.c')
OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)

all: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
