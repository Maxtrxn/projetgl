# Makefile pour le programme d'analyse d'images
CC = gcc
CFLAGS = -O2 -Wall -Wextra `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`
TARGET = benchmark
SOURCES = benchmark.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
