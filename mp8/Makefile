CC=gcc
CFLAGS=-g -Wall -Wextra

mp8: main.o maze.o solutionChecker.o
	$(CC) $(CFLAGS) main.o maze.o solutionChecker.o -o mp8

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

maze.o: maze.c
	$(CC) $(CFLAGS) -c maze.c

clean:
	rm -f main.o maze.o mp8
