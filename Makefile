CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lm

# Liste des fichiers sources
SRCS = main.c propagation.c data.c draw.c network.c utils.c

# Liste des fichiers objets
OBJS = $(SRCS:.c=.o)

all: main

main: $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o main $(LIBS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f main $(OBJS)