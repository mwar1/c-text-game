all: main

main: main.o
	gcc main.o parser.o input.o -o main

main.o: main.c
	gcc -c main.c

parser.o: parser.c parser.h
	gcc -c parser.c

input.o: input.c input.h
	gcc -c input.c

clean:
	rm -f *.o main
