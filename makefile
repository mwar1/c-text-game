all: main

main: main.o parser.o input.o location.o
	gcc main.o parser.o input.o location.o -o main

main.o: main.c
	gcc -c main.c

parser.o: parser.c parser.h
	gcc -c parser.c

input.o: input.c input.h
	gcc -c input.c

location.o: location.c location.h
	gcc -c location.c

clean:
	rm -f *.o main
