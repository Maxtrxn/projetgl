CC = gcc
CFLAGS = -O3 -march=native -Wall -Wextra  
LIBS = -lSDL2 -lm

SRC = main.c types.c affichage.c neurone.c utils.c
OBJ = $(SRC:.c=.o)

EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

%.o: %.c
	$(CC)  $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
