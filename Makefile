CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lm

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LIBS)

clean:
	rm -f main
