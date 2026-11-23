CC = gcc
CFLAGS = -Wall -Wextra -g -o2

main: main.o maze.o ray.o globals.o math_utils.o
	$(CC) $(CFLAGS) -o main main.o maze.o ray.o globals.o math_utils.o
 
main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

maze.o: maze.c maze.h
	$(CC) $(CFLAGS) -c maze.c

ray.o: ray.c ray.h
	$(CC) $(CFLAGS) -c ray.c

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c globals.c

math_utils.o: math_utils.c math_utils.h
	$(CC) $(CFLAGS) -c math_utils.c

