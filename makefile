CC=gcc
CFLAGS=-c -Wall -pedantic -std=c99
LIB=-lm

all: main

main: main.o
	$(CC) main.o -o main $(LIB)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c 

clean:
	rm *.o main