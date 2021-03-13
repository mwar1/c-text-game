#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "location.h"

typedef struct location {
	char tag[20];
	char description[128];
	int connections[4];
	char directions[4];
	int numConnections;
} location;

struct location locs[10] = {{"castle", "a grand, but run down castle", {1, 8, 9, -1}, {'s', 'n', 'w'}},
		            {"bridge", "a rickety, old wooden bridge, swaying in the breeze", {0, 4, -1, -1}, {'n', 's'}},
		            {"observatory", "a small stone hut, with a telescope poking out from a hole in the roof", {7, 8, -1, -1}, {'e', 'w'}},
			    {"lake", "murky water stretching out to the horizon", {4, 6, -1, -1}, {'n', 'e'}},
			    {"cave", "a dank cave, with water running down the walls. A strange light is coming from somewhere", {1, 3, 5, -1}, {'n', 's', 'e'}},
			    {"forest", "dense trees surrounding you. A thick canopy blocks most of the daylight", {4, 6, -1, -1}, {'w', 's'}},
			    {"boathouse", "a run-down wooden boathouse. The door is hanging off the hinges", {3, 5, -1, -1}, {'w', 'n'}},
			    {"greenhouse", "a huge glass greenouse. The windows are covered in moss, casting a peculiar green light", {2, -1, -1, -1}, {'w'}},
			    {"library", "a richly decorated old library, with thousands of ancient books lining the walls", {0, 2, -1, -1}, {'s', 'e'}},
			    {"hall", "a long-forgotten feasting hall from times gone by. A long wooden table still stretches from end to end", {0, -1, -1, -1}, {'e'}}};

int playerLocation = 0;
int numLocs = 10;
location currentLoc;
int numConnections;

void look(char *noun) {
	bool looked = false;
	if (!strcmp(noun, "around")) {
		currentLoc = locs[playerLocation];
		numConnections = sizeof(currentLoc.connections) / sizeof(currentLoc.connections[0]);
		printf("You are in %s.\n", currentLoc.description);
		for (int i=0; i<numConnections; i++) {
			if (currentLoc.connections[i] != -1) {
				if (locs[playerLocation].directions[i] == 'n') {
					printf("To the north you can see a ");
				} else if (locs[playerLocation].directions[i] == 'e') {
					printf("To the east you can see a ");
				} else if (locs[playerLocation].directions[i] == 's') {
					printf("To the south you can see a ");
				} else {
					printf("To the west you can see a ");
				}
				printf("%s\n", locs[currentLoc.connections[i]].tag);
				looked = true;
			}
		}
	}
	if (!looked) {
		printf("I can't see that at the moment.\n");
	}
}

void go(char *noun) {
	bool moved = false;
	numConnections = sizeof(currentLoc.connections) / sizeof(currentLoc.connections[0]);
	currentLoc = locs[playerLocation];
	for (int i=0; i<numConnections; i++) {
		if (!strcmp(noun, locs[locs[playerLocation].connections[i]].tag) || (strlen(noun) == 1 && noun[0] == locs[playerLocation].directions[i])) {
			playerLocation = locs[playerLocation].connections[i];
			printf("Moving to the %s...\n", locs[playerLocation].tag);
			look("around");
			moved = true;
		}
	}
	if (!moved) {
		printf("You can't move there at the moment.\n");
	}
}
