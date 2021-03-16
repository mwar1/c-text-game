#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "location.h"
#include "object.h"

int playerLocation = 0;
int numLocs = 10;
Location *currentLoc;
int numConnections;

Location *locs[10];
Location player = {"player", "you", {-1, -1, -1, -1}, {'x', 'x', 'x', 'x'}};
//struct Object *locObjs[3];

void generateLocations() {
	FILE *locFile = fopen("locations.txt", "r");
	for (int i=0; i<numLocs; i++) {
		locs[i] = malloc(sizeof(Location));

		char *line = NULL;
		size_t n;

		char *tag;
		char *description;
		int connections[4];
		char directions[4];

		getline(&line, &n, locFile);

		tag = strtok(line, "/");
		description = strtok(NULL, "/");
		char *tempConns[] = {strtok(NULL, "/"), strtok(NULL, "/"), strtok(NULL, "/"), strtok(NULL, "/")};
		for (int i=0; i<4; i++) {
			char *tempP;
			long ret;

			ret = strtol(tempConns[i], &tempP, 10);
			connections[i] = ret;
		}

		directions[0] = (char) *strtok(NULL, "/");
		directions[1] = (char) *strtok(NULL, "/");
		directions[2] = (char) *strtok(NULL, "/");
		directions[3] = (char) *strtok(NULL, "\n");

		strcpy(locs[i]->tag, tag);
		strcpy(locs[i]->description, description);
		memcpy(locs[i]->connections, connections, sizeof(int)*4);
		strcpy(locs[i]->directions, directions);
	}
	fclose(locFile);
}

void look(char *noun) {
	bool looked = false;
	if (!strcmp(noun, "around")) {
		currentLoc = locs[playerLocation];
		numConnections = sizeof(currentLoc->connections) / sizeof(currentLoc->connections[0]);
		printf("You are %s.\n\n", currentLoc->description);
		for (int i=0; i<numConnections; i++) {
			if (currentLoc->connections[i] != -1) {
				if (locs[playerLocation]->directions[i] == 'n') {
					printf("To the north you can see a ");
				} else if (locs[playerLocation]->directions[i] == 'e') {
					printf("To the east you can see a ");
				} else if (locs[playerLocation]->directions[i] == 's') {
					printf("To the south you can see a ");
				} else {
					printf("To the west you can see a ");
				}
				printf("%s\n", locs[currentLoc->connections[i]]->tag);
				looked = true;
			}
		}
		printf("\nOn the floor there is:\n");
		bool something = false;
		for (int j=0; j<numObjs; j++) {
			if (!strcmp(objs[j]->location->tag, currentLoc->tag)) {
				something = true;
				printf("a %s\n", objs[j]->tag);
			}
		}
		if (!something) {
			printf("nothing.\n");
		}
	}
	if (!looked) {
		printf("I can't see that at the moment.\n");
	}
}

void go(char *noun) {
	bool moved = false;
	numConnections = sizeof(currentLoc->connections) / sizeof(currentLoc->connections[0]);
	currentLoc = locs[playerLocation];
	for (int i=0; i<numConnections; i++) {
		if (!moved) {
			if (!strcmp(noun, locs[locs[playerLocation]->connections[i]]->tag) || (strlen(noun) == 1 && (int) noun[0] == locs[playerLocation]->directions[i])) {
				playerLocation = locs[playerLocation]->connections[i];
				printf("Moving to the %s...\n", locs[playerLocation]->tag);
				look("around");
				moved = true;
			}
		}
	}
	if (!moved) {
		printf("You can't move there at the moment.\n");
	}
}

void inventory() {
	bool holding = false;
	printf("You are holding:\n");
	for (int i=0; i<numObjs; i++) {
		if (!strcmp(objs[i]->location->tag, "player")) {
			holding = true;
			printf("a %s\n", objs[i]->tag);
		}
	}
	if (!holding) {
		printf("nothing.\n");
	}
}