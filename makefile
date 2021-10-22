all: main

main: main.o parser.o input.o location.o dictionary.o object.o npc.o system.o
	gcc main.o parser.o input.o location.o dictionary.o object.o npc.o system.o -o main

main.o: main.c
	gcc -c main.c

parser.o: parser.c
	gcc -c parser.c

input.o: input.c
	gcc -c input.c

location.o: location.c
	gcc -c location.c

dictionary.o: dictionary.c
	gcc -c dictionary.c

object.o: object.c
	gcc -c object.c

npc.o: npc.c
	gcc -c npc.c

system.o: system.c
	gcc -c system.c

clean:
	rm -f *.o main
