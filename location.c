#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "location.h"
#include "object.h"
#include "npc.h"

int playerLocation = 0;
int numLocs = 20;
int numConnections;

Location *locs[20];
NPC *player;

void generateLocations() {
	FILE *locFile = fopen("locations.txt", "r");
	for (int i=0; i<numLocs; i++) {
		locs[i] = malloc(sizeof(Location));

		char *line = NULL;
		size_t n;

		char *tag;
		char *intro;
		char *description;
		int connections[4];
		char directions[4];

		getline(&line, &n, locFile);

		tag = strtok(line, "/");
		intro = strtok(NULL, "/");
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
		strcpy(locs[i]->intro, intro);
		strcpy(locs[i]->description, description);
		memcpy(locs[i]->connections, connections, sizeof(int)*4);
		strcpy(locs[i]->directions, directions);
		locs[i]->capacity = 9999;
	}
	fclose(locFile);
}

void createPlayer() {
	player = malloc(sizeof(NPC));
	player->super = malloc(sizeof(Location));
	int tempConns[4] = {-1, -1, -1, -1};
	char tempDirs[4] = {'x', 'x', 'x', 'x'};

	strcpy(player->super->tag, "player");
	strcpy(player->super->intro, "you");
	strcpy(player->super->description, "you");
	memcpy(player->super->connections, tempConns, sizeof(int)*4);
	strcpy(player->super->directions, tempDirs);
	player->super->capacity = 30;

	player->location = locs[0];
	strcpy(player->voiceline, ".");
	player->health = 50;
}

void lookAround() {
	numConnections = 0;
	for (int i=0; i<4; i++) {
		if (player->location->connections[i] != -1) {
			numConnections += 1;
		}
	}
	printf("You are %s.\n\n", player->location->description);
	for (int i=0; i<numConnections; i++) {
		if (player->location->connections[i] != -1) {
			if (player->location->directions[i] == 'n') {
				printf("To the north ");
			} else if (player->location->directions[i] == 'e') {
				printf("To the east ");
			} else if (player->location->directions[i] == 's') {
				printf("To the south ");
			} else {
				printf("To the west ");
			}
			printf("%s\n", locs[player->location->connections[i]]->intro);
		}
	}
	
	printf("\nOn the floor there is:\n");
	bool isObject = false;
	for (int j=0; j<numObjs; j++) {
		if (!strcmp(objs[j]->location->tag, player->location->tag)) {
			isObject = true;
			printf("a %s\n", objs[j]->tag);
		}
	}
	if (!isObject) {
		printf("nothing.\n");
	}

	for (int k=0; k<numNPCs; k++) {
		if (!strcmp(npcs[k]->location->tag, player->location->tag)) {
			printf("\nYou can also see %s.\n", npcs[k]->super->description);
		}
	}
}

void go(char *noun) {
	bool moved = false;
	if (noun != NULL) {
		for (int i=0; i<4; i++) {
			if (!moved && player->location->connections[i] != -1 && (strlen(noun) == 1 && (int) noun[0] == player->location->directions[i])) {
				player->location = locs[player->location->connections[i]];
				lookAround();
				moved = true;
			}
		}
	}
	if (!moved) {
		if (strlen(noun) == 1 && ((int) noun[0] == 'n' || (int) noun[0] == 'e' || (int) noun[0] == 's' || (int) noun[0] == 'w')) {
			printf("You can't move in that direction at the moment.\n");
		} else { 
			printf("I don't know how to move in that direction.\n");
		}
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
